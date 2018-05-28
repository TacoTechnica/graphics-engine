
#include "knob.h"

LinearKnob::LinearKnob(double start_frame, double end_frame, double start_val, double end_val) {
    this->start_frame = start_frame;
    this->end_frame = start_frame;
    this->start_val = start_val;
    this->end_val = start_val;
}


bool LinearKnob::isActive(double frame) {
    return (start_frame <= frame && frame <= end_frame);
}

double LinearKnob::getValue(double frame) {
    double progress = (frame - start_frame) / (end_frame - start_frame);
    return start_val + progress * (end_val - start_val);
}
