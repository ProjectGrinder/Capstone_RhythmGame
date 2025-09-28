#pragma once

namespace Game::Battle
{
    enum Instrument
    {
        NO_INSTRUMENT,
    };

    struct BulletHellState
    {
        int graze;
        float iframe_time;
    };

    struct RhythmState
    {
        int judge_perfect;
        int judge_good;
        int judge_fine;
        int judge_miss;
    };

    struct Acceptance
    {
        int total_accept;
        int current_accept;
        int max_accept_gauge;
    };

    struct BattleState
    {
        int hp;
        int score;
        float clock_time;
        Instrument instrument;
        unsigned int difficulty;
        BulletHellState bullet_hell_state;
        RhythmState rhythm_state;
        Acceptance acceptance;
    };
}