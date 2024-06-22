from pathlib import Path
import os, sys, shutil

if __name__ == '__main__':
    release_path = Path("codebase/x64/Release")
    if not release_path.exists():
        print("no release build exists, compile against Release x64 to create this.")
        sys.exit(1)

    content_path = Path("content")
    if not content_path.exists():
        print("no content path exists, make some content folders in content path.")
        sys.exit(2)

    release = Path("release")
    if release.exists():
        print("release folder already exists, resetting it.")
        shutil.rmtree("release")
    print("creating releases folder.")
    os.makedirs("release")

    gameExe = None
    for file in release_path.iterdir():
        if file.name.endswith("pdb"):
            continue
        if file.name.endswith("exe"):
            gameExe = file
        print(f"Copying {file}")
        shutil.copy(file, release)

    shutil.copytree(content_path, release, dirs_exist_ok=True)
    print("Done creating release dir.")

    confirms = [ "y", "j", "yes", "ja", "yeah"]
    decision = input("run exe?")
    for each in confirms:
        if each in decision:
            if gameExe is not None:
                os.system(str(gameExe))
            else:
                print("no executable copied, fix this error.")
                sys.exit(3)
