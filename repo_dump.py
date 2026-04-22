from __future__ import annotations

import argparse
import fnmatch
from pathlib import Path

DEFAULT_EXCLUDE_DIRS = {
    ".git",
    ".hg",
    ".svn",
    ".venv",
    "venv",
    "__pycache__",
    "node_modules",
    ".mypy_cache",
    ".pytest_cache",
    ".ruff_cache",
    ".tox",
    "dist",
    "build",
    ".next",
    ".turbo",
    ".idea",
    ".vscode",
    "target",
    "out",
    ".DS_Store",
}

DEFAULT_EXCLUDE_FILES = {
    "*.pyc",
    "*.pyo",
    "*.so",
    "*.dll",
    "*.dylib",
    "*.exe",
    "*.class",
    "*.jar",
    "*.zip",
    "*.tar",
    "*.gz",
    "*.7z",
    "*.rar",
    "*.pdf",
    "*.png",
    "*.jpg",
    "*.jpeg",
    "*.gif",
    "*.webp",
    "*.ico",
    "*.mp4",
    "*.mp3",
    "*.wav",
    "*.ttf",
    "*.otf",
    "*.woff",
    "*.woff2",
    "*.lock",
}

TEXT_EXTENSIONS = {
    ".py", ".js", ".ts", ".tsx", ".jsx", ".java", ".c", ".h", ".cpp", ".hpp",
    ".rs", ".go", ".rb", ".php", ".swift", ".kt", ".kts", ".scala", ".sh",
    ".bash", ".zsh", ".fish", ".ps1", ".sql", ".html", ".htm", ".css", ".scss",
    ".json", ".jsonc", ".yaml", ".yml", ".toml", ".ini", ".cfg", ".conf",
    ".env", ".md", ".txt", ".rst", ".xml", ".dockerfile", ".gitignore",
    ".gitattributes", ".editorconfig", ".makefile",
}

def is_probably_binary(path: Path, sample_size: int = 4096) -> bool:
    try:
        with path.open("rb") as f:
            chunk = f.read(sample_size)
        if b"\x00" in chunk:
            return True
        if not chunk:
            return False
        # crude heuristic: if too many non-text bytes, treat as binary
        text_chars = bytearray({7, 8, 9, 10, 12, 13, 27} | set(range(32, 127)))
        nontext = sum(byte not in text_chars for byte in chunk)
        return (nontext / len(chunk)) > 0.30
    except Exception:
        return True

def matches_any_pattern(name: str, patterns: set[str]) -> bool:
    return any(fnmatch.fnmatch(name, pattern) for pattern in patterns)

def should_skip(path: Path, root: Path, exclude_dirs: set[str], exclude_files: set[str]) -> bool:
    rel = path.relative_to(root)

    for part in rel.parts:
        if part in exclude_dirs:
            return True

    if path.is_file():
        if path.name in exclude_dirs:
            return True
        if matches_any_pattern(path.name, exclude_files):
            return True

    return False

def collect_files(root: Path, exclude_dirs: set[str], exclude_files: set[str]) -> list[Path]:
    files: list[Path] = []

    for path in root.rglob("*"):
        if should_skip(path, root, exclude_dirs, exclude_files):
            continue
        if path.is_file():
            files.append(path)

    files.sort(key=lambda p: str(p.relative_to(root)).lower())
    return files

def dump_repo(root: Path, output_file: Path, max_file_bytes: int) -> None:
    exclude_dirs = set(DEFAULT_EXCLUDE_DIRS)
    exclude_files = set(DEFAULT_EXCLUDE_FILES)

    files = collect_files(root, exclude_dirs, exclude_files)

    with output_file.open("w", encoding="utf-8", errors="replace") as out:
        out.write(f"REPOSITORY DUMP\n")
        out.write(f"ROOT: {root.resolve()}\n")
        out.write(f"TOTAL FILES: {len(files)}\n")
        out.write("=" * 100 + "\n\n")

        for file_path in files:
            rel = file_path.relative_to(root)

            try:
                size = file_path.stat().st_size
            except OSError:
                continue

            likely_text = (
                file_path.suffix.lower() in TEXT_EXTENSIONS
                or file_path.name in {"Makefile", "Dockerfile", "LICENSE", "README"}
            )

            if not likely_text and is_probably_binary(file_path):
                continue

            out.write("=" * 100 + "\n")
            out.write(f"FILE: {rel}\n")
            out.write(f"SIZE: {size} bytes\n")
            out.write("=" * 100 + "\n")

            try:
                with file_path.open("r", encoding="utf-8", errors="replace") as f:
                    if size > max_file_bytes:
                        content = f.read(max_file_bytes)
                        out.write(content)
                        out.write(
                            f"\n\n[TRUNCATED: file exceeded {max_file_bytes} bytes]\n\n"
                        )
                    else:
                        out.write(f.read())
            except Exception as e:
                out.write(f"[ERROR READING FILE: {e}]\n")

            out.write("\n\n")

def main() -> None:
    parser = argparse.ArgumentParser(
        description="Dump a repository's text files into one .txt file."
    )
    parser.add_argument(
        "repo",
        nargs="?",
        default=".",
        help="Path to the repository root (default: current directory)",
    )
    parser.add_argument(
        "-o",
        "--output",
        default="repo_dump.txt",
        help="Output txt file (default: repo_dump.txt)",
    )
    parser.add_argument(
        "--max-file-bytes",
        type=int,
        default=200_000,
        help="Max bytes to include per file before truncating (default: 200000)",
    )

    args = parser.parse_args()

    root = Path(args.repo).resolve()
    output_file = Path(args.output).resolve()

    if not root.exists() or not root.is_dir():
        raise SystemExit(f"Invalid repo path: {root}")

    dump_repo(root, output_file, args.max_file_bytes)
    print(f"Repo dump written to: {output_file}")

if __name__ == "__main__":
    main()
