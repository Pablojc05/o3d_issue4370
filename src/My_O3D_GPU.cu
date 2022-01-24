#include "My_O3D_GPU.h"

namespace My_O3D_GPU {

    /* ------- Example Class ------ */

    Example::Example(open3d::t::geometry::PointCloud &nube) :_cloud(nube){} 

    Example::~Example(){}
    
     void Example::filter (open3d::core::Tensor &result){

        // 
        open3d::core::Device cuda = open3d::core::Device("CUDA:0");
        open3d::core::Tensor test = open3d::core::Tensor::Init({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}, cuda).To(open3d::core::Dtype::Float32);

        RunParallelForOn(test);

        result = test;
    }
    

} //namespace My_O3D_GPU

void RunParallelForOn(open3d::core::Tensor& tensor) {

    float* test_data = tensor.GetDataPtr<float>();

    auto lambda = [=] OPEN3D_HOST_DEVICE(int64_t i) {
        
        test_data[i]=i*10;              
    };
    open3d::core::ParallelFor(
            tensor.GetDevice(), tensor.NumElements(), lambda);
}

