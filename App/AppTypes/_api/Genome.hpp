#pragma once

#include <vector>

#include <QtTypes>

class RandomCore;

class Genome
{
public:

    inline size_t size() const noexcept { return genType.size(); }
    inline auto operator[](const size_t idx) { return genType[idx]; }
    inline auto operator[](const size_t idx) const { return genType[idx]; }

    [[nodiscard]]
    bool createGenom(const qreal beginSearch, const qreal endSearch,
                     const size_t precission);

    void initGenom(const RandomCore& rand);

    qreal val() const;

private:

    std::vector<bool> genType = {};
    qreal beginSearch = 0.0;
    qreal endSearch = 0.0;
};
