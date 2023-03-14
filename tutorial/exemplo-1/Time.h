#ifndef TIME_H
#define TIME_H

class Time
{
    public:
        Time( int = 0, int = 0, int = 0 ); // declaração do constructor
        void setTime( int, int, int ); // hour, minute, and second
        void setHour( int );
        void setMinute( int );
        void setSecond( int );
        void printUniversal();
        void printStandard();
    private:
        int hour;
        int minute;
        int second;

};

#endif