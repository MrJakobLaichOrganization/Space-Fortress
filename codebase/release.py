from pathlib import Path
import os, sys, shutil


if __name__ == '__main__':
    release_path = Path("x64/Release")
    if not release_path.exists():
        print("no release build exists")
        sys.exit(1)
    
    content_path = Path("content")
    if not content_path.exists():
        print("no content path exists")
        sys.exit(1)
    
    release = Path("release")
    if not release.exists():
        os.makedirs("release")

    for file in release_path.iterdir():
        if file.name.endswith("pdb"):
            continue
        print(f"Copying {file}")
        shutil.copy(file, release)

    shutil.copytree(content_path, release, dirs_exist_ok=True)
    print("Done.")
