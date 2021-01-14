// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include <core/graph/graph_viewer.h>

#include "model_builder.h"

namespace onnxruntime {
namespace coreml {

ModelBuilder::ModelBuilder(const GraphViewer& graph_viewer)
    : graph_viewer_(graph_viewer) {
  coreml_model_ = onnxruntime::make_unique<CoreML::Specification::Model>();
  (void)graph_viewer_;
}

}  // namespace coreml
}  // namespace onnxruntime
