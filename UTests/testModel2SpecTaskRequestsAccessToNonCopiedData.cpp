///////////////////////////////////////////////////////////////////////////
// Spetabaru - Berenger Bramas MPCDF - 2017
// Under LGPL Licence, please you must read the LICENCE file.
///////////////////////////////////////////////////////////////////////////

#include "UTester.hpp"

#include "Utils/SpModes.hpp"
#include "Utils/SpUtils.hpp"

#include "Tasks/SpTask.hpp"
#include "Runtimes/SpRuntime.hpp"

class TestModel2SpecTaskRequestAccessToNonCopiedData : public UTester<TestModel2SpecTaskRequestAccessToNonCopiedData> {
    using Parent = UTester<TestModel2SpecTaskRequestAccessToNonCopiedData>;

    void Test(){
        int a=0, b=0;
        SpRuntime<SpSpeculativeModel::SP_MODEL_2> runtime(SpUtils::DefaultNumThreads());

        runtime.potentialTask(SpMaybeWrite(a), [](int &param_a) -> bool{
            (void) param_a;
            return false;
        });
        
        runtime.potentialTask(SpRead(a), SpMaybeWrite(b), [](const int &param_a, int &param_b) -> bool{
            (void) param_a;
            (void) param_b;
            return false;
        });
        
        runtime.waitAllTasks();
        runtime.stopAllThreads();
    }

    void SetTests() {
        Parent::AddTest(&TestModel2SpecTaskRequestAccessToNonCopiedData::Test, 
                        "Test behavior when speculative model 2 is used and a to be inserted speculative task requests access to non copied data");
    }
};

// You must do this
TestClass(TestModel2SpecTaskRequestAccessToNonCopiedData)