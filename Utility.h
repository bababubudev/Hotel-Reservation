#pragma once
#include <cstdlib>
#include <ctime>

/* header file for keeping all the important functions */

/* inline function because the function doesnt need to be looked at as the contents are small and the overhead would be slower otherwise */
inline int reserve_id()
{
	/* static cast is used to so compiler can compile which can help performance */
	/* usin cstdlib srand takes time to use psuedo random algorithm and returns unique result */
	srand(static_cast<int>(time(nullptr)));
	/* between 10 000 to 99 999 */
	return rand() % 90000 + 10000;
}

inline int discount_amount()
{
	/* similarly here but returns result from 1-3 multiplied by 10 */
	srand(static_cast<int>(time(nullptr)));
	return rand() % 3 * 10;
}

inline int room_division()
{
	// static_cast makes the psuedo random too predictable.
	srand((int)(time(nullptr)));
	return rand() % MAX_ROOMS / 4 + MIN_ROOMS;
}

#pragma region Input Validation

/* template is used in case input other than an 64bit int is passed through */
template<typename T>
void validated_input(T& input, int min = 0, int max = 0)
{
	/* using iomanup i can force the input to ignore anything above a width of one */
	std::cin >> std::setw(1) >> input;

	/* comparisons are not done if min and max are not set */
	const bool comparable = !(min == max);

	/* checks whether the input is in between int parameters min and max */
	bool in_between = input >= min && input <= max;

	/* loop runs while input is either invalid or is comparable and not inbetween the specified range */
	while (!std::cin.good() || (!in_between && comparable))
	{
		/* runs when input is valid but not between specified values */
		if (std::cin.good())
		{
			std::cout << "\n[ Only numbers from " << min << " : " << max << " are valid! ]\n\n";

			std::cin >> std::setw(1) >> input;
			in_between = input >= min && input <= max;
		}
		/* runs when input is not valid */
		else
		{
			std::cin.clear();
			std::cin.ignore(INT_MAX, '\n');

			std::cout << "\n[ Only numbers are allowed! ]\n\n";

			std::cin >> std::setw(1) >> input;
			in_between = input >= min && input <= max;
		}
	}

	std::cin.clear();
	std::cin.ignore(INT_MAX, '\n');
}

/* Overload function for the one above, calls the function and returns a value */
template<typename T>
T validated_input(const int min = 0, const int max = 0)
{
	T input;
	validated_input<T>(input, min, max);

	return input;
}

/* boolean is used incase the string can contain a number */
inline void validated_input(std::string& input, const bool num_allowed = false)
{
	bool success = false;
	std::getline(std::cin, input);

	/* returns early */
	if (num_allowed) return;

	/* success is true if input is not empty and does not contain a number */
	while (!success)
	{
		try
		{
			std::stoi(input);
			std::cout << "\n[ Name cannot start with a number ]\n\n";
			std::getline(std::cin, input);
		}
		catch (const std::exception&)
		{
			if (input.empty())
			{
				std::cout << "\n[ Please enter your name before you continue ]\n\n";
				std::cin.clear();
				std::getline(std::cin, input);
				continue;
			}

			success = true;
		}
	}
}

/* overload for the above function, returns a string */
inline std::string validated_input(const bool num_allowed = false)
{
	std::string input;
	validated_input(input, num_allowed);

	return input;
}

#pragma endregion