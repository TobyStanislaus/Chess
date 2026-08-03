from pathlib import Path
import re


def find_current_version():
    ROOT = Path(__file__).resolve().parent.parent
    path = ROOT / "data"

    version_pattern = re.compile(r"^version(\d+)$")

    versions = []
    for entry in path.iterdir():
        if entry.is_dir():
            match = version_pattern.match(entry.name)
            if match:
                versions.append(int(match.group(1)))

    if not versions:
        return 0  # no version folders exist yet

    return max(versions)