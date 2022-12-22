import os
import subprocess

# this updates the git repos according to the pattern the dawn build expects
def git_update(url: str, sha: str) -> None:
    original_cwd = os.getcwd()
    os.chdir("vulkan-deps")
    repo_name = url.split("/")[-1]
    print("BEFORE >>>>> ", repo_name)
    if repo_name.endswith(".git"):
        repo_name = repo_name.rstrip(".git")
    print(">>>>> ", repo_name)

    if os.path.exists(repo_name):
        os.chdir(repo_name + "/src")
        subprocess.run(["git", "pull"])
        os.chdir(os.pardir)
    else:
        subprocess.run(["git", "clone", url, repo_name + "/src"])
    os.chdir(repo_name + "/src")
    if sha == "HEAD":
        subprocess.run(["git", "checkout", "master"])
    else:
        subprocess.run(["git", "checkout", sha])
    os.chdir(original_cwd)

path = "vulkan-deps-src"
if not os.path.exists("vulkan-deps"):
    os.makedirs("vulkan-deps")
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

