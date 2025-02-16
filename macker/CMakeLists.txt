add_library(MyClangPlugin SHARED
  main.cpp
  MyPPCallbacks.cpp
  # MyASTConsumer.cpp
  # MyFrontendAction.cpp
)

# LLVM/Clang ライブラリをリンク（バージョンによって必要なライブラリは多少異なる場合があります）
target_link_libraries(MyClangPlugin
  clangBasic
  clangFrontend
  clangAST
  clangASTMatchers
  clangLex
  clangParse
  clangSema
  clangEdit
  clangDriver
  clangRewrite
  clangSerialization
  clangTooling
  clangAnalysis
  # 必要に応じて追加
  )

# macOS の場合、プラグインを .so ではなく .dylib として扱う
set_target_properties(MyClangPlugin PROPERTIES
  PREFIX ""
  )

