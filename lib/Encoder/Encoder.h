#ifndef Encoder_h
#define Encoder_h

class Encoder
{
public:
    Encoder(int pinA, int pinB, double min, double max, double delta);
    void begin();
    void step();
    double getValue();
private:
    int pinA;
    int pinB;
    double value = 0;
    double min;
    double max;
    double delta;
    unsigned char state;
};

#endif