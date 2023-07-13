# LabSlang

LabSlang is currently a .gitmodules file and some scripts
that result in a reliable dawn/tint build set up.

## Prerequisites

- Python 3.9 or greater
- jinja2 needs to be installed via pip or conda

## First time

Clone recursively, then run `python update_vulkan_deps.py` ~ python will
need to be at least version 3.9.

At the moment, dawn doesn't actually build with the versions set in its
`vulkan-deps`. Until that's fixed, please go into the 
`vulkan-deps/Vulkan-Heades/src` directory and check out the `v1.3.238` tag.

## Subsequently

Use cmake to build and install the project. You may include `LabSlang` as a
subproject in your own projects. Please note that dawn is a complex project,
and if you take it on, whether via LabSlang, or other means, you'll need a 
certain familiarity with setting up cmake projects.


