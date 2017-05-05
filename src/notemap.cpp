#include "notemap.h"

#include <cstdio>
#include <cstring>

#include "mathhelper.h"

#include "debug.h"

NoteMap::NoteMap()
{

}

bool NoteMap::loadTjaFile(const char *filename)
{
    strcpy(tjafile, filename);
    if(!reloadTjaFile())
    {
        cout<<"reload tja file failed"<<endl;
        return false;
    }
    return true;
}

BeatTimeVec NoteMap::offsetMapState(MapState &state, double deltaSec) const
{
    double originBeat=state.beatOffset;
    double originDeltaSec=deltaSec;

    if(state.currentSegment<(int)segments.size())
    {
        auto iter=segments.cbegin()+state.currentSegment;
        for(; iter!=segments.cend()&&deltaSec>0/*deltaSec>=M_DINFS*/; ++iter)
        {
            deltaSec=iter->offsetMapState(state, deltaSec);
        }
    }

//    if(deltaSec>=M_DINFS)
    if(deltaSec>0)
    {
        state.timeOffset+=deltaSec;
        state.beatOffset+=state.calcBeatOffset(deltaSec);
    }

    return BeatTimeVec(originDeltaSec, state.beatOffset-originBeat);
}

BeatTimeVec NoteMap::offsetMapStateSingle(MapState& state, double deltaSec) const
{
    BeatTimeVec originTime=state.toBeatTime();

    if(state.currentSegment<(int)segments.size())
    {
        deltaSec=segments[state.currentSegment].offsetMapStateSingleEvent(state, deltaSec);
    }
    else if(deltaSec>0)
    {
        state.timeOffset+=deltaSec;
        state.beatOffset+=state.calcBeatOffset(deltaSec);
    }

    return BeatTimeVec(state.timeOffset-originTime.time, state.beatOffset-originTime.beat);
}

int NoteMap::getNoteInfo(list<NoteInfo>& infoList, int maxinum)
{
    int loadedNum=0;
    while(loadProc!=segments.end() && maxinum!=0)
    {
        loadProc->getNoteInfo(infoList);
        maxinum--;
        loadedNum++;
        loadProc++;
    }
    return loadedNum;
}

const MapState& NoteMap::getBeginState()
{
    return beginState;
}

MapState NoteMap::getBgmBeginState()
{
    MapState result=beginState;
    result.beatOffset-=result.calcBeatOffset(result.timeOffset);
    result.timeOffset=0.0;
    return std::move(result);
}

MapState NoteMap::getStateByOffset(double offset)
{
    if(offset<beginState.timeOffset)
    {
        MapState result=beginState;
        result.beatOffset-=result.calcBeatOffset(result.timeOffset-offset);
        result.timeOffset=offset;
        return std::move(result);
    }
    MapState result=beginState;
    offsetMapState(result, offset-result.timeOffset);
    return std::move(result);
}

const char* NoteMap::getWavFilename()
{
    return wavfile;
}

bool NoteMap::reloadTjaFile()
{
    FILE* f=fopen(tjafile, "r");
    if(!f)return false;

    char line[500];

    int i,j,k;
    char c,*p;
    bool flag;
    double x;

    SegmentNote tempNote;
    SegmentEvent tempEvent;

    bool readingNotes=false;
    bool readingEnded=false;

    Segment currentSegment;
    int currentNoteCount=0;

    rewind(f);

    segments.clear();

    while(fgets(line,500,f) && !readingEnded)
    {
        p=strstr(line, "//");
        if(p)*p=0;
        i=strlen(line);
        while(i>0&&(line[i-1]==' '||line[i-1]=='\n'||line[i-1]=='\r'))i--;
        line[i]=0;

        if(i==0)continue;

        if(!readingNotes)
        {
            if(line[0]=='#')
            {
                if(strcmp(line, "#START")==0)
                {
                    readingNotes=true;
                    currentSegment.init(&beginState);
                }
            }
            else
            {
                p=strstr(line, ":");
                if(p)
                {
                    *p++=0;
                    while(*p==' ')p++;
                    if(strcmp(line, "WAVE")==0)
                    {
                        strcpy(wavfile, p);
                    }
                    else if(strcmp(line, "TITLE")==0)
                    {
                        strcpy(title, p);
                    }
                    else if(strcmp(line, "SUBTITLE")==0)
                    {
                        if(strstr(p, "--")==p)
                        {
                            p+=2;
                            showSub=false;
                        }
                        strcpy(subtitle, p);
                    }
                    else if(strcmp(line, "OFFSET")==0)
                    {
                        sscanf(p, "%lf", &x);
                        beginState.timeOffset=-x;
                    }
                    else if(strcmp(line, "BPM")==0)
                    {
                        sscanf(p, "%lf", &x);
                        beginState.bpm=x;
                    }
                }
            }
        }
        else
        {
            if(line[0]=='#')
            {
                memset(&tempEvent, 0, sizeof(SegmentEvent));
                tempEvent.num=currentNoteCount;
                tempEvent.type=unknown;

                flag=true;

                if(strncmp(line, "#END", 4)==0)
                {
                    flag=false;
                    readingNotes=false;
                    readingEnded=true;
                }
                else if(strncmp(line, "#BPMCHANGE", 10)==0)
                {
                    tempEvent.type=bpmchange;
                    sscanf(line+11, "%lf", &x);
                    tempEvent.parameter_d=x;
                }
                else if(strncmp(line, "#MEASURE", 8)==0)
                {
                    tempEvent.type=measure;
                    sscanf(line+9, "%d/%d", &j, &k);
                    tempEvent.parameter_i_1=j;
                    tempEvent.parameter_i_2=k;
                }
                else if(strncmp(line, "#SCROLL", 7)==0)
                {
                    tempEvent.type=scroll;
                    sscanf(line+8, "%lf", &x);
                    tempEvent.parameter_d=x;
                }
                else
                {
                    flag=false;
                }

                if(flag)
                    currentSegment.appendEvent(tempEvent);

            }
            else
            {
                for(i=0; line[i]; ++i)
                {
                    c=line[i];
                    if(c==',')
                    {
                        currentSegment.setSegmentDiv(currentNoteCount);
                        segments.push_back(currentSegment);
                        currentSegment.nextSegmentState();
                        currentSegment.init();
                        currentNoteCount=0;
                    }
                    else if((c>='0' && c<='9')||(c>='A' && c<='Z')||(c>='a' && c<='z'))
                    {
                        if(c!='0')
                        {
                            tempNote.num=currentNoteCount;
                            tempNote.type=(NoteType)(c-'0');
                            currentSegment.appendNote(tempNote);
                        }
                        ++currentNoteCount;
                    }
                }
            }
        }
    }

    if(ferror(f))
    {
        cout<<"error while tja file reading"<<endl;
        return false;
    }

    fclose(f);

    loadProc=segments.begin();

    return true;

}
