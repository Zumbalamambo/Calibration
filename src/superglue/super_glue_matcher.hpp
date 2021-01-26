#ifndef _SUPER_POINT_MATCHER_HPP_
#define _SUPER_POINT_MATCHER_HPP_

#include <string>
#include <vector>
#include <array>

#include "opencv2/core.hpp"
#include "qttorch.h"

#include "extract_common.hpp"

#include "super_glue.hpp"
#include "tensor_rt_utils.hpp"
#include "libtorch_utils.hpp"


namespace marker
{
    class SuperGlueMatcher
    {
    public:
        static constexpr int IMAGE_COUNT = 2;

    public:
        SuperGlueMatcher(const std::string& engine_path, tensor_rt_utils::Logger& logger);
        int keypoint_size() const;
        int score_size() const;
        int descr_size() const;
        void match(const KeypointSetArray& src);
        const MatchTable& output() const;

    public:
        struct PerformanceStats
        {
            std::chrono::microseconds binding_resize_duration;
            std::chrono::microseconds keypoint_count_duration;
            std::chrono::microseconds forward_duration;
            std::size_t call_count;

            PerformanceStats();
        };
        void performance_test_match(const KeypointSetArray& src, PerformanceStats& perf_stats);

    private:
        SuperGlue m_super_glue;
        MatchTable m_output;
    };
}

#endif  //   _SUPER_POINT_MATCHER_HPP_
