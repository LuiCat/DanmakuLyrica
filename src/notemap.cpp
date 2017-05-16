#include "notemap.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <unordered_map>

#include "mathhelper.h"

#include "debug.h"

using namespace std;

NoteMap::NoteMap()
{

}

bool NoteMap::loadTjaFile(const char *filename)
{
    tjafile=filename;
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

const string& NoteMap::getWavFilename()
{
    return wavfile;
}

bool NoteMap::reloadTjaFile()
{
    ifstream f(tjafile.c_str());
    if(!f)return false;

    string line;

    int i,j,k;
    char c;// , *p;
    bool flag;
    double x;

    //unordered_map<string, string> props;

    SegmentNote tempNote;
    SegmentEvent tempEvent;

    bool readingNotes=false;
    bool readingEnded=false;

    Segment currentSegment;
    int currentNoteCount=0;

    segments.clear();

    while(!f.eof() && !readingEnded)
    {
        getline(f, line);

        i=line.rfind("//");
        if(i!=string::npos)line.resize(i);

        i=line.length();
        while(i>0&&isspace(line[i-1]))i--;
        line.resize(i);

        if(i==0)continue;

        if(!readingNotes)
        {
            if(line[0]=='#')
            {
                if(line=="#START")
                {
                    readingNotes=true;
                    currentSegment.init(&beginState);
                }
            }
            else
            {
                i=line.find(':');
                if(i!=string::npos)
                {
                    string key=line.substr(0, i);
                    ++i;
                    while(i<line.length()&&line[i]==' ')++i;
                    string value=line.substr(i);

                    if(key=="WAVE")
                    {
                        wavfile=value;
                    }
                    else if(key=="TITLE")
                    {
                        title=value;
                    }
                    else if(key=="SUBTITLE")
                    {
                        if(value.find("--")==0)
                            subtitle=value.substr(2);
                        else
                            subtitle=value;
                    }
                    else if(key=="OFFSET")
                    {
                        beginState.timeOffset=-atof(value.c_str());
                    }
                    else if(key=="BPM")
                    {
                        beginState.bpm=atof(value.c_str());
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

                if(line=="#END")
                {
                    readingNotes=false;
                    readingEnded=true;
                }
                else if(line.compare(0, 10, "#BPMCHANGE")==0)
                {
                    tempEvent.type=bpmchange;
                    tempEvent.parameter_d=atof(line.c_str()+10);
                }
                else if(line.compare(0, 8, "#MEASURE")==0)
                {
                    tempEvent.type=measure;
                    sscanf(line.c_str()+8, "%d/%d", &j, &k);
                    tempEvent.parameter_i_1=j;
                    tempEvent.parameter_i_2=k;
                }
                else if(line.compare(0, 7, "#SCROLL")==0)
                {
                    tempEvent.type=scroll;
                    tempEvent.parameter_d=atof(line.c_str()+7);
                }

                if(tempEvent.type!=unknown)
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

    if(f.fail())
    {
        cout<<"error while tja file reading"<<endl;
        return false;
    }

    loadProc=segments.begin();

    return true;

}
