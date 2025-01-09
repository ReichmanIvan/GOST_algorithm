#include <iostream>
#include <random>
#include <stdexcept>

// Функция для целочисленного возведения в степень
unsigned long integerPow(unsigned long base, unsigned long exp)
{
    unsigned long result = 1;
    while (exp > 0)
    {
        if (exp % 2 == 1)
            result *= base;
        base *= base;
        exp /= 2;
    }
    return result;
}

// Функция для умножения чисел по модулю
unsigned long multiplyMod(unsigned long a, unsigned long b, unsigned long mod);

// Функция для быстрого возведения в степень по модулю
unsigned long powerMod(unsigned long base, unsigned long exp, unsigned long mod);

// Функция для вычисления наибольшего общего делителя
unsigned long gcd(unsigned long x, unsigned long y);

// Функция для определения количества цифр в числе
unsigned long numberLength(unsigned long num);

// Тест Ферма для проверки числа на простоту
bool fermatTest(unsigned long number);

// Генерация простого числа заданной длины на основе ГОСТ
unsigned long generatePrime(unsigned long length, unsigned long primeBase);

int main()
{
    setlocale(LC_ALL, "Russian");
    try
    {
        unsigned long length, primeBase;
        std::cout << "Введите длину числа: ";
        std::cin >> length;
        std::cout << "Введите простое число длиной " << length / 2 << ": ";
        std::cin >> primeBase;

        unsigned long prime = generatePrime(length, primeBase);
        std::cout << "Сгенерировано простое число: " << prime;

        if (fermatTest(prime))
            std::cout << " - это простое число." << std::endl;
    }
    catch (const std::exception& ex)
    {
        std::cerr << "Ошибка: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

unsigned long multiplyMod(unsigned long a, unsigned long b, unsigned long mod)
{
    if (b == 1)
        return a;
    if (b % 2 == 0)
    {
        unsigned long half = multiplyMod(a, b / 2, mod);
        return (2 * half) % mod;
    }
    return (multiplyMod(a, b - 1, mod) + a) % mod;
}

unsigned long powerMod(unsigned long base, unsigned long exp, unsigned long mod)
{
    if (exp == 0)
        return 1;
    if (exp % 2 == 0)
    {
        unsigned long half = powerMod(base, exp / 2, mod);
        return multiplyMod(half, half, mod);
    }
    return multiplyMod(powerMod(base, exp - 1, mod), base, mod);
}

unsigned long gcd(unsigned long x, unsigned long y)
{
    while (y != 0)
    {
        unsigned long temp = x % y;
        x = y;
        y = temp;
    }
    return x;
}

unsigned long numberLength(unsigned long num)
{
    unsigned long length = 0;
    do
    {
        ++length;
        num /= 10;
    } while (num > 0);
    return length;
}

bool fermatTest(unsigned long number)
{
    if (number <= 3)
        throw std::invalid_argument("Число должно быть больше 3");

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<unsigned long> dist(2, number - 2);

    for (size_t i = 0; i < 100; ++i)
    {
        unsigned long randomBase = dist(generator);
        if (gcd(randomBase, number) != 1 || powerMod(randomBase, number - 1, number) != 1)
            return false;
    }
    return true;
}

unsigned long generatePrime(unsigned long length, unsigned long primeBase)
{
    if (!fermatTest(primeBase))
        throw std::invalid_argument("Число не является простым.");

    if (numberLength(primeBase) != length / 2)
        throw std::invalid_argument("Длина базового числа не соответствует требованиям.");

    unsigned long factor = integerPow(10, length - 1) / primeBase + (integerPow(10, length - 1) % primeBase != 0);
    if (factor % 2 == 0)
        ++factor;

    unsigned long offset = 0;
    while (true)
    {
        unsigned long candidate = primeBase * (factor + offset) + 1;
        if (candidate >= integerPow(10, length))
        {
            ++factor;
            offset = 0;
            continue;
        }

        if (fermatTest(candidate))
            return candidate;

        offset += 2;
    }
}
