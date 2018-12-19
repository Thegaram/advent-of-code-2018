#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <string>
#include <vector>

using reg = std::array<int, 6>;

// --------- addition ---------

reg addr(reg rs, int A, int B, int C)
{
    rs[C] = rs[A] + rs[B];
    return rs;
}

reg addi(reg rs, int A, int B, int C)
{
    rs[C] = rs[A] + B;
    return rs;
}

// --------- multiplication ---------

reg mulr(reg rs, int A, int B, int C)
{
    rs[C] = rs[A] * rs[B];
    return rs;
}

reg muli(reg rs, int A, int B, int C)
{
    rs[C] = rs[A] * B;
    return rs;
}

// --------- bitwise AND ---------

reg banr(reg rs, int A, int B, int C)
{
    rs[C] = rs[A] & rs[B];
    return rs;
}

reg bani(reg rs, int A, int B, int C)
{
    rs[C] = rs[A] & B;
    return rs;
}

// --------- bitwise OR ---------

reg borr(reg rs, int A, int B, int C)
{
    rs[C] = rs[A] | rs[B];
    return rs;
}

reg bori(reg rs, int A, int B, int C)
{
    rs[C] = rs[A] | B;
    return rs;
}

// --------- asignment ---------

reg setr(reg rs, int A, int B, int C)
{
    rs[C] = rs[A];
    return rs;
}

reg seti(reg rs, int A, int B, int C)
{
    rs[C] = A;
    return rs;
}

// --------- greater-than testing ---------

reg gtir(reg rs, int A, int B, int C)
{
    rs[C] = A > rs[B];
    return rs;
}

reg gtri(reg rs, int A, int B, int C)
{
    rs[C] = rs[A] > B;
    return rs;
}

reg gtrr(reg rs, int A, int B, int C)
{
    rs[C] = rs[A] > rs[B];
    return rs;
}

// --------- equality testing ---------

reg eqir(reg rs, int A, int B, int C)
{
    rs[C] = A == rs[B];
    return rs;
}

reg eqri(reg rs, int A, int B, int C)
{
    rs[C] = rs[A] == B;
    return rs;
}

reg eqrr(reg rs, int A, int B, int C)
{
    rs[C] = rs[A] == rs[B];
    return rs;
}

// --------- opcodes ---------

enum class Opcode {
    ADDR, ADDI,
    MULR, MULI,
    BANR, BANI,
    BORR, BORI,
    SETR, SETI,
    GTIR, GTRI, GTRR,
    EQIR, EQRI, EQRR
};

// --------- execution ---------

struct Inst
{
    std::string OP;
    int A, B, C;
};

reg execute(reg rs, Inst i)
{
    static std::map<std::string, std::function<reg(reg, int, int, int)>> op = {
        { "addr", addr }, { "addi", addi },
        { "mulr", mulr }, { "muli", muli },
        { "banr", banr }, { "bani", bani },
        { "borr", borr }, { "bori", bori },
        { "setr", setr }, { "seti", seti },
        { "gtir", gtir }, { "gtri", gtri }, { "gtrr", gtrr },
        { "eqir", eqir }, { "eqri", eqri }, { "eqrr", eqrr }
    };

    assert(op.count(i.OP) > 0);
    return op[i.OP](rs, i.A, i.B, i.C);
}

// --------- solution ---------

std::pair<int, std::vector<Inst>> read(std::istream& is)
{
    std::regex ip_rgx {R"(^#ip (\d+)$)"};
    std::regex inst_rgx {R"(^(\w+) (\d+) (\d+) (\d+)$)"};

    std::string line;
    std::getline(is, line);
    std::smatch m;
    assert(std::regex_search(line, m, ip_rgx));
    int p = std::stoi(m[1]);

    std::vector<Inst> instructions;

    while (true)
    {
        std::getline(is, line);
        if (is.eof()) break;

        std::smatch m;
        if (!std::regex_search(line, m, inst_rgx))
            break;

        std::string op = m[1];
        int A = std::stoi(m[2]);
        int B = std::stoi(m[3]);
        int C = std::stoi(m[4]);

        instructions.push_back({op, A, B, C});
    }

    return {p, instructions};
}

std::ostream& operator<<(std::ostream& os, reg rs)
{
    auto [r0, r1, r2, r3, r4, r5] = rs;
    os << "[" << r0 << ", " << r1 << ", " << r2 << ", " << r3 << ", " << r4 << ", " << r5 << "]";
    return os;
}

int part2()
{
    int R0 = 0;
    int R2 = 0;
    int R4 = 1;
    int R5 = 10551381;

    do {

        R2 = 1;

        do {

            // if (R2 * R4 == R5)
            //     R0 += R4;
            // R2 += 1;

            if (R5 % R4 == 0) R0 += R4;
            R2 = R5 + 1;

        } while (R2 <= R5);

        R4 += 1;

    } while(R4 <= R5);

    return R0; // result: 14266944 (sum of divisors of 10551381)
}

int main()
{
    auto [p, instructions] = read(std::cin);

    reg rs = {0, 0, 0, 0, 0, 0};
    int& ip = rs[p];

    while (ip < instructions.size())
    {
        rs = execute(rs, instructions[ip]);
        ++ip;
    }

    std::cout << rs << std::endl;

    // std::cout << part2() << std::endl;

    return 0;
}