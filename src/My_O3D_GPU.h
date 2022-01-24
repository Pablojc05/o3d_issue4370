#ifndef My_O3D_GPU_H
#define My_O3D_GPU_H

#include <open3d/Open3D.h>
#include <vector>

#include <cuda.h>
#include <cuda_runtime.h>

#ifdef __CUDACC__
    // #include "open3d/core/CUDAUtils.h" // already included in Parallelfor.h
    #include "open3d/core/ParallelFor.h"
#endif  

#include <stdio.h>
#include <thread>


namespace My_O3D_GPU {


    class Example {

        public:

            Example(open3d::t::geometry::PointCloud &nube);
            virtual ~Example();

            void filter(open3d::core::Tensor &result);

        private:
            open3d::t::geometry::PointCloud _cloud;

    };

} //namespace My_O3D_GPU

void RunParallelForOn(open3d::core::Tensor& tensor);

#endif