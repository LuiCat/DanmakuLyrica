#ifndef NOTEJUDGE_H
#define NOTEJUDGE_H

enum JudgeResult
{
    Judge_Ignore = 0,
    Judge_Miss = 1,
    Judge_Bad = 2,
    Judge_Cool = 3,
    Judge_Great = 4
};

class NoteJudge
{
public:

    NoteJudge();

    JudgeResult judgeNote(double offsetSec);

private:

    double rangeBad;
    double rangeGood;
    double rangeGreat;

};

#endif // NOTEJUDGE_H
