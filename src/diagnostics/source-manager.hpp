#pragma once

#include <memory>
#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include <fstream>
#include <sstream>

namespace Cosylang::Source
{

struct FileBuffer
{
    std::string file_name;
    std::string source_code;
    std::vector<size_t> line_offsets;
    size_t file_id;

    FileBuffer(std::string this_name = "", std::string this_source = "", size_t this_id = 0) :
        file_name(this_name), file_id(this_id) {source_code = formatSource(this_source);}

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

    std::string formatSource(std::string source)
    {
        source.push_back('\0');
        std::string new_source = "";
        int i = 0;
        while (source.at(i) != '\0')
        {
            if (source.at(i) == '\t')
            {
                i++;
                continue;
            }
            new_source.push_back(source.at(i));
            i++;
        }
        return new_source;
    }

    void buildFileLineOffsets()
    {
        line_offsets.push_back(0);
        for (size_t i = 0; i < source_code.size(); ++i)
        {
            if (source_code[i] == '\n')
            {
                line_offsets.push_back(i + 1);
            }
        }
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

    SourceManager(std::vector<std::unique_ptr<FileBuffer>> this_files)
        : files(std::move(this_files)) {}

    std::optional<size_t> loadFile(const std::string& filepath)
    {
        std::ifstream in(filepath);
        if (!in)
        {
            return std::nullopt;
        }

        std::stringstream stream;
        stream << in.rdbuf();
        std::string file_content = stream.str();

        auto buffer = std::make_unique<FileBuffer>();
        buffer->file_name = filepath;
        buffer->file_id = next_file_id++;
        buffer->source_code = std::move(file_content);
        buildLineOffsets(*buffer);

        size_t loaded_id = buffer->file_id;
        files.push_back(std::move(buffer));
        return loaded_id;
    }

    FileBuffer* getFileBuffer(size_t id) const
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
