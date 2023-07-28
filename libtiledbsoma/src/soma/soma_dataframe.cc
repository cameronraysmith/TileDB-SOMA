/**
 * @file   soma_dataframe.cc
 *
 * @section LICENSE
 *
 * The MIT License
 *
 * @copyright Copyright (c) 2023 TileDB, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 *   This file defines the SOMADataFrame class.
 */

#include "soma_dataframe.h"
#include <tiledb/tiledb>
#include "array_buffers.h"
#include "soma_array.h"

namespace tiledbsoma {
using namespace tiledb;

//===================================================================
//= public static
//===================================================================

std::shared_ptr<SOMADataFrame> SOMADataFrame::create(
    std::string_view uri,
    ArraySchema schema,
    std::map<std::string, std::string> platform_config) {
    return SOMADataFrame::create(
        uri, schema, std::make_shared<Context>(Config(platform_config)));
}

std::shared_ptr<SOMADataFrame> SOMADataFrame::create(
    std::string_view uri, ArraySchema schema, std::shared_ptr<Context> ctx) {
    SOMAArray::create(ctx, uri, schema, "SOMADataFrame");
    return SOMADataFrame::open(uri, OpenMode::read, ctx);
}

std::shared_ptr<SOMADataFrame> SOMADataFrame::open(
    std::string_view uri,
    OpenMode mode,
    std::map<std::string, std::string> platform_config,
    std::vector<std::string> column_names,
    ResultOrder result_order,
    std::optional<std::pair<uint64_t, uint64_t>> timestamp) {
    return SOMADataFrame::open(
        uri,
        mode,
        std::make_shared<Context>(Config(platform_config)),
        column_names,
        result_order,
        timestamp);
}

std::shared_ptr<SOMADataFrame> SOMADataFrame::open(
    std::string_view uri,
    OpenMode mode,
    std::shared_ptr<Context> ctx,
    std::vector<std::string> column_names,
    ResultOrder result_order,
    std::optional<std::pair<uint64_t, uint64_t>> timestamp) {
    return std::make_shared<SOMADataFrame>(
        mode, uri, ctx, column_names, result_order, timestamp);
}

//===================================================================
//= public non-static
//===================================================================

SOMADataFrame::SOMADataFrame(
    OpenMode mode,
    std::string_view uri,
    std::shared_ptr<Context> ctx,
    std::vector<std::string> column_names,
    ResultOrder result_order,
    std::optional<std::pair<uint64_t, uint64_t>> timestamp) {
    array_ = std::make_shared<SOMAArray>(
        mode,
        uri,
        "unnamed",  // name
        ctx,
        column_names,
        "auto",  // batch_size,
        result_order,
        timestamp);
    array_->reset();
    array_->submit();
}

void SOMADataFrame::open(
    OpenMode mode, std::optional<std::pair<uint64_t, uint64_t>> timestamp) {
    array_->open(mode, timestamp);
    array_->reset();
    array_->submit();
}

void SOMADataFrame::close() {
    array_->close();
}

const std::string SOMADataFrame::uri() const {
    return array_->uri();
}

std::shared_ptr<Context> SOMADataFrame::ctx() {
    return array_->ctx();
}

std::shared_ptr<ArraySchema> SOMADataFrame::schema() const {
    return array_->schema();
}

const std::vector<std::string> SOMADataFrame::index_column_names() const {
    return array_->dimension_names();
}

int64_t SOMADataFrame::count() const {
    return array_->ndim();
}

std::optional<std::shared_ptr<ArrayBuffers>> SOMADataFrame::read_next() {
    return array_->read_next();
}

void SOMADataFrame::write(std::shared_ptr<ArrayBuffers> buffers) {
    array_->reset();
    array_->submit();
    array_->write(buffers);
}

}  // namespace tiledbsoma
