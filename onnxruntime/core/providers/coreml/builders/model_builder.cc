// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "model_builder.h"
#include "op_builder_factory.h"

namespace onnxruntime {
namespace coreml {

ModelBuilder::ModelBuilder(const GraphViewer& graph_viewer)
    : graph_viewer_(graph_viewer) {
  coreml_model_ = onnxruntime::make_unique<CoreML::Specification::Model>();
  (void)graph_viewer_;
}

}  // namespace coreml
}  // namespace onnxruntime
