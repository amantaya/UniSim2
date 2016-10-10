#ifndef SEED_BANK_H
#define SEED_BANK_H
#include <QMap>
#include <QVector>
#include <base/box.h>

namespace awe {

class SeedBank : public base::Box
{
public:
    SeedBank(QString name, QObject *parent=0);
	
    void initialize();
    void reset();
	void update();
	
private:
    // inputs
    int dayOfYear;
    double initial, yearlyEmergence, yearlyMortality, cropLaiExp, cropLai;
    QVector<double> emergenceByMonth;

    // pull variables
    double nonDormant, dormant, total,
        dailyEmergenceRatio, totalEmergenceRatio,
        dailyEmergenceDensity, totalEmergenceDensity,
        dailyEmergenceRatioPotential, cropEffectOnEmergence;

    // push variables
    double dormantInflow, instantMortality;


    // decoded parameters
    double dailySurvivalRate, emergenceScaling;
    QMap<int,double> emergenceCalendar;    // indexed by mid-month day of the year from Dec to Jan

    // methods
    void initEmergenceCalendar();
    double lookupEmergence(int dayOfYear) const;
    void fitEmergence();
    void scaleEmergence(double factor);
    double calcTotalEmergenceRatio() const;
    double calcCropEffectOnEmergence() const;
    void addInflow();
    void applyInstantMortality();
};


}
#endif
