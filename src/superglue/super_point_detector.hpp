#ifndef _SUPER_POINT_DETECTOR_HPP_
#define _SUPER_POINT_DETECTOR_HPP_

#include <string>
#include <vector>
#include <array>
#include <chrono>

#include "opencv2/core.hpp"
#include "opencv2/cudaarithm.hpp"
#include "opencv2/cudaimgproc.hpp"
#include "opencv2/cudawarping.hpp"

#include "qttorch.h"

#include "extract_common.hpp"
#include "super_point.hpp"
#include "tensor_rt_utils.hpp"
#include "libtorch_utils.hpp"
#include "gpu_mat_batch.hpp"

namespace marker
{
    class SuperPointDetector
    {
    public:
        static constexpr int IMAGE_COUNT = 2;

    public:
        SuperPointDetector(const std::string& engine_path, tensor_rt_utils::Logger& logger);
        const tensor_rt_utils::Dims3d& input_shape() const;
        tensor_rt_utils::Dims4d scores_shape() const;
        tensor_rt_utils::Dims4d descriptors_shape() const;
        // void detect(const cv::Mat& first, const cv::Mat& first_mask, 
        //     const cv::Mat& second, const cv::Mat& second_mask);

        void detect(const cv::Mat& first, const cv::Mat& second);

        const ScoreMap& score_map() const;
        const DescriptorMap& descriptor_map() const;

    public:
        struct PerformanceStats
        {
            std::chrono::microseconds preprocessing_duration;
            std::chrono::microseconds forward_duration;
            std::size_t call_count;

            PerformanceStats();
        };
        void performance_test_detect(const cv::Mat& first, const cv::Mat& second, 
            PerformanceStats& perf_stats);

    private:
        SuperPoint m_super_point;
        tensor_rt_utils::Dims3d m_input_shape;
        std::array<cv::cuda::GpuMat, IMAGE_COUNT> m_gpu_input;      
        GpuMatBatch m_gpu_uint8;
        GpuMatBatch m_gpu_float;
        // std::array<at::Tensor, IMAGE_COUNT> m_input_masks;

        // at::Tensor m_score_map;
        // at::Tensor m_score_mask;
        // at::Tensor m_inner_area;
        // at::Tensor m_descr_map;

        // KeypointSetArray m_output;
        ScoreMap m_score_map;
        DescriptorMap m_descr_map;
    };
}

#endif // _SUPER_POINT_DETECTOR_HPP_
