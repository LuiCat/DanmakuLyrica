#ifndef SCORE_H
#define SCORE_H


class Score
{
public:

    Score();

    void setCombo(int x);
    inline int getCombo() const
    {
        return combo;
    }

    void addScore(int x);
    void addRawScore(int x);
    inline int getScore() const
    {
        return score;
    }

    bool isUpdated();

protected:

    int combo;
    int multiplier;
    int score;

    bool updated;

};

#endif // SCORE_H
