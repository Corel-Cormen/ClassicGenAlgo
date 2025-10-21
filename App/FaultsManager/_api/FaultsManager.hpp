#include <array>

#include "FaultsManagerInterface.hpp"

class FaultsManager : public FaultsManagerInterface
{
public:
    FaultsManager();

    void updateFault(Faults::FaultId fault, bool status) override;

    FaultsVec getFaults() override;

    bool isNonRecoverable() override;

    bool isRecoverable() override;

    void clearRecoverableFaults() override;

private:

    struct FaultProp
    {
        bool isCritical;
        bool isActive;
    };

    std::array<FaultProp, Faults::FaultId::FAULTS_LENGTH> faultsArray = {};
};
