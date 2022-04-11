#pragma once

#include <string>

// Execute multiple commands in a batch.

namespace BatchCommand {
    void Run(
        const std::wstring& open_file,                // source PDF document
        const std::wstring& password,                 // open password
        const std::wstring& save_file,                // path to save PDF docuemnt
        const std::wstring& batch_config,             // batch command configuration JSON
        std::ostream& output
        );
}
