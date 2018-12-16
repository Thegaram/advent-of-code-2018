#include <array>
#include <cassert>
#include <functional>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <tuple>
#include <vector>

using reg = std::array<int, 4>;

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

std::array<Opcode, 16> ops = {
    Opcode::ADDR, Opcode::ADDI,
    Opcode::MULR, Opcode::MULI,
    Opcode::BANR, Opcode::BANI,
    Opcode::BORR, Opcode::BORI,
    Opcode::SETR, Opcode::SETI,
    Opcode::GTIR, Opcode::GTRI, Opcode::GTRR,
    Opcode::EQIR, Opcode::EQRI, Opcode::EQRR
};

// --------- execution ---------

struct Inst
{
    Opcode OP;
    int A, B, C;
};

reg execute(reg rs, Inst i)
{
    std::function<reg(reg, int, int, int)> op;

    switch (i.OP)
    {
        case Opcode::ADDR: op = addr; break;
        case Opcode::ADDI: op = addi; break;
        case Opcode::MULR: op = mulr; break;
        case Opcode::MULI: op = muli; break;
        case Opcode::BANR: op = banr; break;
        case Opcode::BANI: op = bani; break;
        case Opcode::BORR: op = borr; break;
        case Opcode::BORI: op = bori; break;
        case Opcode::SETR: op = setr; break;
        case Opcode::SETI: op = seti; break;
        case Opcode::GTIR: op = gtir; break;
        case Opcode::GTRI: op = gtri; break;
        case Opcode::GTRR: op = gtrr; break;
        case Opcode::EQIR: op = eqir; break;
        case Opcode::EQRI: op = eqri; break;
        case Opcode::EQRR: op = eqrr; break;
    }

    return op(rs, i.A, i.B, i.C);
}

reg execute_m(reg rs, const std::vector<Inst>& instructions)
{
    for (Inst i : instructions)
        rs = execute(rs, i);
    return rs;
}

// --------- solution ---------

struct RawInst
{
    int OP, A, B, C;
};

struct Sample
{
    reg before;
    reg after;
    RawInst i;
};

std::pair<std::vector<Sample>, std::vector<RawInst>> read(std::istream& is)
{
    std::regex regex {R"(^Before:\s+\[(\d+), (\d+), (\d+), (\d+)\](\d+) (\d+) (\d+) (\d+)After:\s+\[(\d+), (\d+), (\d+), (\d+)\]$)"};
    std::vector<Sample> samples;

    while (true)
    {
        std::string line1, line2, line3;
        std::getline(std::cin, line1);
        std::getline(std::cin, line2);
        std::getline(std::cin, line3);
        if (std::cin.eof()) break;

        std::string entry = line1 + line2 + line3;

        std::smatch m;
        if (!std::regex_search(entry, m, regex))
            break;

        int r00 = std::stoi(m[1]), r01 = std::stoi(m[2]),  r02 = std::stoi(m[3]),  r03 = std::stoi(m[4]);
        int OP  = std::stoi(m[5]), A   = std::stoi(m[6]),  B   = std::stoi(m[7]),  C   = std::stoi(m[8]);
        int r10 = std::stoi(m[9]), r11 = std::stoi(m[10]), r12 = std::stoi(m[11]), r13 = std::stoi(m[12]);

        samples.push_back({{r00, r01, r02, r03}, {r10, r11, r12, r13}, {OP, A, B, C}});

        std::string dummy;
        std::getline(std::cin, dummy);
    }

    std::vector<RawInst> instructions;

    while (true)
    {
        int OP, A, B, C;
        std::cin >> OP >> A >> B >> C;
        if (std::cin.eof()) break;
        instructions.push_back({OP, A, B, C});
    }

    return {samples, instructions};
}

std::map<int, Opcode> find_mapping(const std::vector<Sample>& samples)
{
    std::map<int, std::set<Opcode>> options;
    std::set<Opcode> known;
    std::map<int, Opcode> code2op;

    auto note = [&known, &code2op](int code, Opcode op){
        known.insert(op);
        assert (!code2op.count(code) || code2op[code] == op);
        code2op[code] = op;
    };

    for (int ii = 0; ii < samples.size(); ++ii)
    {
        const Sample& s = samples[ii];

        for (Opcode op : ops)
            if (execute(s.before, {op, s.i.A, s.i.B, s.i.C}) == s.after)
                options[ii].insert(op);

        if (options[ii].size() == 1)
            note(s.i.OP, *std::begin(options[ii]));
    }

    while (known.size() < ops.size())
    {
        for (int ii = 0; ii < samples.size(); ++ii)
        {
            if (options[ii].size() == 1)
                continue;

            std::set<Opcode> to_remove;
            for (auto op : options[ii])
                if (known.count(op) > 0)
                    to_remove.insert(op);

            for (Opcode op : to_remove)
                options[ii].erase(op);

            if (options[ii].size() == 1)
                note(samples[ii].i.OP, *std::begin(options[ii]));
        }
    }

    return code2op;
}

int count_ambiguous(const std::vector<Sample>& samples)
{
    return std::count_if(std::begin(samples), std::end(samples), [](Sample s){
        int count = std::count_if(std::begin(ops), std::end(ops), [s](Opcode op) {
            return execute(s.before, {op, s.i.A, s.i.B, s.i.C}) == s.after;
        });
        return count >= 3;
    });
}

int main()
{
    auto [samples, raw] = read(std::cin);

    // part 1
    std::cout << "count = " << count_ambiguous(samples) << std::endl;

    // part 2
    auto code2op = find_mapping(samples);

    std::vector<Inst> instructions(raw.size());
    std::transform(std::begin(raw), std::end(raw), std::begin(instructions), [&code2op](RawInst ri) {
        return Inst{code2op[ri.OP], ri.A, ri.B, ri.C};
    });

    auto [r0, r1, r2, r3] = execute_m({0, 0, 0, 0}, instructions);
    std::cout << "[" << r0 << ", " << r1 << ", " << r2 << ", " << r3 << "]" << std::endl;

    return 0;
}