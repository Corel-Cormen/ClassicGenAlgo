#pragma once

#include <vector>

#include <QtTypes>

class RandomCore;

class Genome
{
public:

    enum class GenomeValType
    {
        BINARY_GENOME_TYPE = 0,
        REAL_VALUE_GENOME_TYPE,
    };

    Genome() = delete;
    Genome(GenomeValType genomeValType);
    Genome(const Genome& other);
    Genome(Genome&& other) noexcept;

    Genome& operator=(const Genome& other);
    Genome& operator=(Genome&& other) noexcept;

    inline size_t size() const noexcept { return genType.size(); }
    inline auto operator[](const size_t idx) { return genType[idx]; }
    inline auto operator[](const size_t idx) const { return genType[idx]; }

    [[nodiscard]]
    bool createGenom(const qreal beginSearch, const qreal endSearch,
                     const size_t precission);

    void initGenom(const RandomCore& rand);

    qreal val() const;

    inline void setValue(const qreal value) noexcept { realValue = value; }

    inline qreal getMinVal() const { return beginSearch; }
    inline qreal getMaxVal() const { return endSearch; }

private:

    const GenomeValType genomeValType;
    std::vector<bool> genType = {};
    qreal beginSearch = 0.0;
    qreal endSearch = 0.0;
    qreal realValue = 0.0;
};
