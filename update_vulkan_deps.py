import os
import subprocess
import sys

print("note that the specified vulkan header tag is for v1.2, but dawn itself")
print("uses vulkan 1.3 features. checking out Vulkan-Headers v1.3.328 works.")

if sys.version_info < (3, 9):
    print("Python 3.9 or greater is required")
    sys.exit()

if len(sys.argv) == 1:
    basepath = "./"
else:
    basepath = sys.argv[1:][0] + "/"

path = basepath + "vulkan-deps-src"

print("source path is ", path)

if not os.path.exists(basepath + "vulkan-deps"):
    os.makedirs(basepath + "vulkan-deps")

# this updates the git repos according to the pattern the dawn build expects
def git_update(url: str, sha: str) -> None:
    original_cwd = os.getcwd()
    os.chdir(basepath + "vulkan-deps")
    repo_name = url.split("/")[-1]
    #print("BEFORE >>>>> ", repo_name)
    repo_name = repo_name.removesuffix(".git")
    #if repo_name.endswith(".git"):
    #    repo_name = repo_name.rstrip(".git")
    #print(">>>>> ", repo_name)

    if os.path.exists(repo_name):
        os.chdir(repo_name + "/src")
        subprocess.run(["git", "pull"])
        os.chdir(os.pardir)
        os.chdir(os.pardir)
    else:
        subprocess.run(["git", "clone", url, repo_name + "/src"])
    os.chdir(repo_name + "/src")
    if sha == "HEAD":
        subprocess.run(["git", "checkout", "master"])
    else:
        subprocess.run(["git", "checkout", sha])
    os.chdir(original_cwd)

for root, dirs, files in os.walk(path):
    if root == path:
        continue

    print("------------------\n", root, path, "\n-------------------\n")
    if "README.chromium" in files:
        SHA = "HEAD"
        URL = None
        with open(os.path.join(root, "README.chromium")) as f:
            lines = f.readlines()
            for line in lines:
                if line.startswith("Revision:"):
                    _, SHA = line.split(":")
                    SHA = SHA.strip()
                elif line.startswith("URL:"):
                    _, URL = line.split(":", 1)
                    URL = URL.strip().rstrip('/')
            if URL:
                git_update(URL, SHA)

