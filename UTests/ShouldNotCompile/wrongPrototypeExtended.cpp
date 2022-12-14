///////////////////////////////////////////////////////////////////////////
// Specx - Berenger Bramas MPCDF - 2017
// Under LGPL Licence, please you must read the LICENCE file.
///////////////////////////////////////////////////////////////////////////
#include "Config/SpConfig.hpp"
#include "Data/SpDataAccessMode.hpp"
#include "Utils/SpUtils.hpp"

#include "Task/SpTask.hpp"
#include "Legacy/SpRuntime.hpp"

// @NBTESTS = 7

int main(){
    const int NumThreads = SpUtils::DefaultNumThreads();
    SpRuntime runtime(NumThreads);

    const double initVal = 1.0;
    double writeVal = 0.0;
    
#ifdef TEST1
    runtime.task(SpRead(initVal),
                []([[maybe_unused]] const double& initValParam){},
                []([[maybe_unused]] const double& initValParam){});
#endif

#ifdef TEST2
    runtime.task(SpRead(initVal),
                SpCpu([]([[maybe_unused]] const double& initValParam){}),
                SpCpu([]([[maybe_unused]] const double& initValParam){}));
#endif

#ifdef TEST3
    runtime.task(SpRead(initVal),
            SpCuda([]([[maybe_unused]] const double& initValParam){}),
            SpCuda([]([[maybe_unused]] const double& initValParam){}));
#endif

#ifdef TEST4
    if constexpr(!SpConfig::CompileWithCuda) {
        runtime.task(SpRead(initVal),
                SpCuda([]([[maybe_unused]] const double& initValParam){}));
    } else {
        static_assert(false, "Force wrongPrototypeExtended-4 to fail.");
    } 
#endif

#ifdef TEST5
    runtime.task(SpProbability(0.25), SpRead(initVal), SpWrite(writeVal),
                 SpCpu([]([[maybe_unused]] const double& initValParam, double& writeValParam) {
                    writeValParam = 42;
                 }));
#endif

    runtime.waitAllTasks();
    runtime.stopAllThreads();
    
    SpTaskGraph<SpSpeculativeModel::SP_NO_SPEC> tg;
    
#ifdef TEST6
    tg.task(SpProbability(0.5), SpRead(initVal), SpCpu([](const double&p) {}));
#endif

#idef TEST7
    tg.task(SpPriority(10), SpRead(initVal), SpPotentialWrite(writeVal),
        SpCpu([](const auto& rParam, auto& wParam) -> bool {
            if(rParam == 0.5) {
                wParam = 2.0;
                return true;
            }
            
            return false;
        });
#endif
}
