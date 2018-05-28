#ifndef H_KNOB
#define H_KNOB


class Knob {
    public:
        Knob(){}
        // =0; makes it abstract + virtual
        virtual double getValue(double frame)=0; // Current value, from a frame
        virtual bool isActive(double frame)=0; // Are we active on this frame?
};

class LinearKnob : public Knob {
    private:
        double start_frame, end_frame;
        double start_val,   end_val;
    public:
        LinearKnob(double start_frame, double end_frame, double start_val, double end_val) : Knob();
        double getValue(double frame);
        bool isActive(double frame);
};

//class DefaultKnob : public Knob {
//    public:
//        DefaultKnob() : Knob(); 
//        double getValue(double frame) { return 1; }
//        bool isActive(double frame) { return true; }
//};

#endif
