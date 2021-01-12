// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "coreml_execution_provider.h"

#include "core/framework/allocatormgr.h"
#include "core/framework/compute_capability.h"
#include "core/graph/graph_viewer.h"
#include "core/session/onnxruntime_cxx_api.h"

namespace onnxruntime {

constexpr const char* COREML = "CoreML";

CoreMLExecutionProvider::CoreMLExecutionProvider()
    : IExecutionProvider{onnxruntime::kCoreMLExecutionProvider} {
  AllocatorCreationInfo device_info(
      [](int) {
        return onnxruntime::make_unique<CPUAllocator>(OrtMemoryInfo(COREML, OrtAllocatorType::OrtDeviceAllocator));
      });

  InsertAllocator(CreateAllocator(device_info));

  AllocatorCreationInfo cpu_memory_info(
      [](int) {
        return onnxruntime::make_unique<CPUAllocator>(
            OrtMemoryInfo(COREML, OrtAllocatorType::OrtDeviceAllocator, OrtDevice(), 0, OrtMemTypeCPUOutput));
      });

  InsertAllocator(CreateAllocator(cpu_memory_info));
}

CoreMLExecutionProvider::~CoreMLExecutionProvider() {
  // Need to delete the temp coreml files
}

std::vector<std::unique_ptr<ComputeCapability>>
CoreMLExecutionProvider::GetCapability(const onnxruntime::GraphViewer& /* graph_view */,
                                       const std::vector<const KernelRegistry*>& /*kernel_registries*/) const {
  std::vector<std::unique_ptr<ComputeCapability>> result;

  return result;
}

common::Status CoreMLExecutionProvider::Compile(const std::vector<FusedNodeAndGraph>& fused_nodes_and_graphs,
                                                std::vector<NodeComputeInfo>& node_compute_funcs) {
  for (const auto& fused_node_and_graph : fused_nodes_and_graphs) {
    ORT_UNUSED_PARAMETER(fused_node_and_graph);
    NodeComputeInfo compute_info;
    compute_info.create_state_func = [](ComputeContext* /*context*/, FunctionState* /*state*/) { return 0; };
    compute_info.release_state_func = [](FunctionState /*state*/) {};
    compute_info.compute_func = [](FunctionState /* state */, const OrtCustomOpApi* /* api */,
                                   OrtKernelContext* /* context */) {
      return ORT_MAKE_STATUS(ONNXRUNTIME, NOT_IMPLEMENTED, "Compute is not supported in this build.");
    };
    node_compute_funcs.push_back(compute_info);
  }
  return Status::OK();
}

}  // namespace onnxruntime
