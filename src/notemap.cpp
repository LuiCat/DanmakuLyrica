#include "notemap.h"

#include <cstdio>
#include <cstring>

#include "debug.h"

Notemap::Notemap()
{
}

bool Notemap::loadTjaFile(const char *filename)
{
    strcpy(tjafile, filename);
    if(!reloadTjaFile())
    {
        cout<<"reload tja file failed"<<endl;
        return false;
    }
    return true;
}

bool Notemap::reloadTjaFile()
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
                        beginState.offset=-x;
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
                tempEvent.type=SegmentEvent::unknown;

                flag=true;

                if(strcmp(line, "#END")==0)
                {
                    flag=false;
                    readingNotes=false;
                    readingEnded=true;
                }
                else if(strcmp(line, "#BPMCHANGE")==0)
                {
                    tempEvent.type=SegmentEvent::bpmchange;
                    sscanf(p, "%lf", &x);
                    tempEvent.param_d=x;
                }
                else if(strcmp(line, "#MEASURE")==0)
                {
                    tempEvent.type=SegmentEvent::measure;
                    sscanf(p, "%d/%d", &j, &k);
                    tempEvent.param_i_1=j;
                    tempEvent.param_i_2=k;
                }
                else if(strcmp(line, "#SCROLL")==0)
                {
                    tempEvent.type=SegmentEvent::scroll;
                    sscanf(p, "%lf", &x);
                    tempEvent.param_d=x;
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
                    else if(c>='0' && c<='9')
                    {
                        if(c!='0')
                        {
                            tempNote.type=(SegmentNote::NoteType)(c-'0');
                            currentSegment.appendNote(tempNote);
                        }
                        ++tempNote.num;
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

    return true;

}
