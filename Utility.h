#pragma once
#include <cstdlib>
#include <ctime>

inline int reserve_id()
{
	srand(static_cast<int>(time(nullptr)));
	return rand() % 90000 + 10000;
}

inline int discount_amount()
{
	srand(static_cast<int>(time(nullptr)));
	return rand() % 3 * 10;
}

inline int room_division()
{
	srand((int)(time(nullptr)));
	return rand() % MAX_ROOMS / 4 + MIN_ROOMS;
}

#pragma region Input Validation
template<typename T>
void validated_input(T& input, int min = 0, int max = 0)
{
	std::cin >> std::setw(1) >> input;
	const bool comparable = !(min == max);

	bool in_between = input >= min && input <= max;

	while (!std::cin.good() || (!in_between && comparable))
	{
		if (std::cin.good())
		{
			std::cout << "\n[ Only numbers from " << min << " : " << max << " are valid! ]\n\n";

			std::cin >> std::setw(1) >> input;
			in_between = input >= min && input <= max;
		}
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


template<typename T>
T validated_input(const int min = 0, const int max = 0)
{
	T input;
	validated_input<T>(input, min, max);

	return input;
}

inline void validated_input(std::string& input, const bool num_allowed = false)
{
	bool success = false;
	std::getline(std::cin, input);

	if (num_allowed) return;

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

inline std::string validated_input(const bool num_allowed = false)
{
	std::string input;
	validated_input(input, num_allowed);

	return input;
}

#pragma endregion