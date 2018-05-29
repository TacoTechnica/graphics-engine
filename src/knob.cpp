
#include<stdio.h>
#include "knob.h"

LinearKnob::LinearKnob(double start_frame, double end_frame, double start_val, double end_val) {
    this->start_frame = start_frame;
    this->end_frame = end_frame;
    this->start_val = start_val;
    this->end_val = end_val;
}


bool LinearKnob::isActive(double frame) {
    return (start_frame <= frame && frame <= end_frame);
}

double LinearKnob::getValue(double frame) {
    printf("[knob.cpp] getValue Frame: %lf\n", frame);
    double progress = (frame - start_frame) / (end_frame - start_frame);
    return start_val + progress * (end_val - start_val);
}
