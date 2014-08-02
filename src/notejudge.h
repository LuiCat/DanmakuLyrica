#ifndef NOTEJUDGE_H
#define NOTEJUDGE_H

enum JudgeResult
{
    miss,
    bad,
    good,
    great
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
