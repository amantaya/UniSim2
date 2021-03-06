#ifndef BASE_RANDOM_GENERATOR_H
#define BASE_RANDOM_GENERATOR_H
#include <QObject>
#include <boost/random/mersenne_twister.hpp>

namespace base {

class RandomGenerator : public QObject
{
public: 
    typedef boost::mt19937 Generator;
    friend RandomGenerator::Generator* randomGenerator();
private:
    RandomGenerator();
    Generator generator;
    static RandomGenerator *theRandomGenerator;
};

RandomGenerator::Generator* randomGenerator();

} //namespace
#endif
