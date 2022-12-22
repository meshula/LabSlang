import os
import subprocess

def git_update(url: str, sha: str) -> None:
    original_cwd = os.getcwd()
    os.chdir("vulkan-deps")
    repo_name = url.split("/")[-1]
    if os.path.exists(repo_name):
        os.chdir(repo_name)
        subprocess.run(["git", "pull"])
        os.chdir(os.pardir)
    else:
        subprocess.run(["git", "clone", url])
    os.chdir(repo_name)
    if sha == "HEAD":
        subprocess.run(["git", "checkout", "main"])
    else:
        subprocess.run(["git", "checkout", sha])
    os.chdir(original_cwd)

path = "vulkan-deps-src"
if not.os.path.exists("vulkan-deps"):
    os.makedirs("vulkan-deps")
for root, dirs, files in os.walk(path):
    print("------------------\n", root, "\n-------------------\n")
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

