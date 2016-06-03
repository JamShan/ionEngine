#!/usr/bin/env bash

cd "${0%/*}"

function CloneGithubDependency {
	repoName="$1"
	repoURL="$2"
	revision="$3"

	echo "Cloning Dependency '$repoName'"
	echo "================================================================================"

	rm -rf "./$repoName"
	git clone "$repoURL" "$repoName"
	cd "./$repoName"
	git reset --hard "$revision"
	cd ..

	echo
}

CloneGithubDependency 'stb'            'https://github.com/nothings/stb.git'         'c9ead07188b342350530e92e14542222c3ad9abe'
CloneGithubDependency 'tinyformat'     'https://github.com/c42f/tinyformat.git'      '3a33bbf65442432277eee079e83d3e8fac51730c'
CloneGithubDependency 'glm'            'https://github.com/g-truc/glm.git'           '8f0d85460557dc5fbf4f44dc23a7708d065d7683'
CloneGithubDependency 'imgui'          'https://github.com/ocornut/imgui.git'        '8291d7dc7c3d17425462e7aaf8593a4465405968'
CloneGithubDependency 'Catch'          'https://github.com/philsquared/Catch.git'    'f666f5f0aef121c357a3705b0187e77920a3e0b5'
CloneGithubDependency 'glad'           'https://github.com/iondune/glad.git'         '410adfead3468c13d40120a5259e687b8ef0616e'
CloneGithubDependency 'tinyobjloader'  'https://github.com/syoyo/tinyobjloader.git'  'ad9911ef1b124c628581c0f75adb3178f9d03c3c'
CloneGithubDependency 'simpleini'      'https://github.com/brofield/simpleini.git'   'e850d4e1cae65ee10f60992c5a576dce7fe35220'
