#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <string>

class EmployeeComponent;

/**
 * @class Mediator
 * @brief Abstract mediator that lets the members of a team communicate.
 *
 * @par Design pattern
 * Mediator (Mediator); the concrete mediator is Team and the colleagues are
 * EmployeeComponent objects.
 *
 * @see Team, EmployeeComponent
 */
class Mediator {

public:
	/** @brief Virtual destructor. */
	virtual ~Mediator() {};

	/**
	 * @brief Passes one member's message on to the others.
	 * @param employeeComponent The sender.
	 * @param message The message text.
	 */
	virtual void notifyColleagues(EmployeeComponent* employeeComponent, std::string message) = 0;
};

#endif
