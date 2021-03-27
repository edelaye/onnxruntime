// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "registry.h"

namespace onnxruntime {
DECLARE_QDQ_CREATOR(Conv, QDQConvTransformer)
DECLARE_QDQ_CREATOR(MaxPool, QDQSimpleTransformer)
DECLARE_QDQ_CREATOR(Reshape, QDQSimpleTransformer)
DECLARE_QDQ_CREATOR(Add, QDQBinaryOpTransformer)
DECLARE_QDQ_CREATOR(Mul, QDQBinaryOpTransformer)
DECLARE_QDQ_CREATOR(MatMul, QDQMatMulTransformer)
std::unordered_map<std::string, QDQRegistry::QDQTransformerCreator> QDQRegistry::qdqtransformer_creators_{
    REGISTER_QDQ_CREATOR(Conv, QDQConvTransformer),
    REGISTER_QDQ_CREATOR(MaxPool, QDQSimpleTransformer),
    REGISTER_QDQ_CREATOR(Reshape, QDQSimpleTransformer),
    REGISTER_QDQ_CREATOR(Add, QDQBinaryOpTransformer),
    REGISTER_QDQ_CREATOR(Mul, QDQBinaryOpTransformer),
    REGISTER_QDQ_CREATOR(MatMul, QDQMatMulTransformer),
};
}  // namespace onnxruntime