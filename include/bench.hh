#pragma once

/** Query an elapsed time timer.
 *
 * This function returns a value representing the number
 * of microseconds elapsed since some unspecified epoch.
 */
class Bench {
  protected:

    std::ostream     &os;
    const int         N;
    const int         R;
    const std::string op;
    bool              reported;

    clock_t           t0;
    clock_t           tF;
    clock_t           dt;

    double            tfac;
    double            usea;

  public:

    Bench(std::ostream     &os,   //
          const int         N,    //
          const int         R,    //
          const std::string op,   //
          bool              reported = false)
            : os(os)
            , N(N)
            , R(R)
            , op(op)
            , reported(reported)

            , t0(-1)
            , tF(-1)
            , dt(-1)

            , tfac(-1)
            , usea(-1)

    {
        tfac = 1.0;               // ... dt*tfac is clocks
        tfac /= CLOCKS_PER_SEC;   // ... dt*tfac is seconds
        tfac *= 1.0e9;            // ... dt*tfac is ns
        tfac /= N;                // ... dt*tfac is ns per N
        tfac /= R;                // ... dt*tfac is ns per N per R

        restart();
    }

    void restart()
    {
        tF = -1;
        dt = -1;
        usea = -1;
        t0 = clock();
    }

    double nsec_ea()
    {
        if (usea < 0) {
            tF = clock();
            dt = tF - t0;
            usea = dt * tfac;
        }
        return usea;
    }

    void report()
    {
        double t = nsec_ea();
        os << std::setw(15)          //
           << std::setprecision(3)   //
           << std::fixed << t        //
           << " " << op << std::endl;
        reported = true;
    }

    ~Bench()
    {
        if (!reported)
            report();
    }
};
