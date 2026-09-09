#pragma once

#include <memory>
#include <string>
#include <vector>
#include <optional>

namespace Cosylang::Source
{

struct FileBuffer
{
    std::string source_code;
    std::vector<size_t> line_offsets;
    size_t file_id;

    std::string_view getLine(size_t line_number) const
    {
        if (line_number == 0 || line_number > line_offsets.size())
        {
            return {};
        }
        size_t start = line_offsets[line_number - 1];
        size_t end = (line_number < line_offsets.size()) ? line_offsets[line_number] - 1 : source_code.size();
        return std::string_view(source_code.data() + start, end - start);
    }
};

class SourceManager
{
    size_t next_file_id = 0;
    std::vector<std::unique_ptr<FileBuffer>> files;

    void buildLineOffsets(FileBuffer& buffer)
    {
        buffer.line_offsets.push_back(0);
        for (size_t i = 0; i < buffer.source_code.size(); ++i)
        {
            if (buffer.source_code[i] == '\n')
            {
                buffer.line_offsets.push_back(i + 1);
            }
        }
    }

public:
    std::optional<size_t> loadFile(const std::string& filepath)
    {
        std::string file_content;
        if (file_content.empty())
        {
            return std::nullopt;
        }

        auto buffer = std::make_unique<FileBuffer>();
        buffer->file_id = {next_file_id++};
        buffer->source_code = std::move(file_content);
        buildLineOffsets(*buffer);

        size_t loaded_id = buffer->file_id;
        files.push_back(std::move(buffer));
        return loaded_id;
    }

    const FileBuffer* getFileBuffer(size_t id) const
    {
         for (const auto& buffer_ptr : files)
         {
             if (buffer_ptr->file_id == id)
             {
                 return buffer_ptr.get();
             }
         }
         return nullptr;
     }
};

}; // namespace Cosylang::Source
