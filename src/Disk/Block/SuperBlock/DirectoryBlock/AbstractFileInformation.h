#pragma once
#include <string>

using std::string;

/**
 * \brief This is an abstract class of file information for different file allocation method.
 */
class AbstractFileInformation
{
protected:
    int fileName = -1;

public:
    AbstractFileInformation() = default;
    virtual ~AbstractFileInformation() = default;

    int getFileName() const;
    void setFileName(int fileName);

    virtual string toString() const = 0;

    AbstractFileInformation(const AbstractFileInformation &) = default;
    AbstractFileInformation &operator=(const AbstractFileInformation &) = default;
    AbstractFileInformation(AbstractFileInformation &&) = default;
    AbstractFileInformation &operator=(AbstractFileInformation &&) = default;
};
