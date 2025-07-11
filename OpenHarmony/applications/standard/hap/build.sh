#!/bin/bash

# Copyright (c) 2022 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
#  limitations under the License.

set -e

CUR_PATH=$(cd $(dirname ${BASH_SOURCE[0]}) && pwd)
BASE_PATH=$(dirname ${CUR_PATH})
ROOT_PATH=$(cd ${CUR_PATH}/../../.. && pwd) && cd -

arg_project=""
arg_sdk_path=""
arg_build_sdk="false"
arg_help="0"
arg_url=""
arg_branch=""
arg_npm=""
arg_ohpm=""
arg_out_path="${ROOT_PATH}/out/hap"
arg_sign_tool="${ROOT_PATH}/developtools/hapsigner/dist"
arg_p7b=""
arg_apl="normal"
arg_feature="hos_normal_app"
arg_profile="UnsgnedReleasedProfileTemplate.json"
arg_bundle_name=""

ohos_sdk_path="${ROOT_PATH}/out/sdk/packages/ohos-sdk/linux"

function print_help() {
  cat <<EOF
  use assembleHap [options] <mainclass> [args...]

EOF
}


function clear_dir(){
        if [ ! -d "$1" ]; then
                mkdir -p $1
        fi
}


function is_project_root(){
        if [ -f $1"/build-profile.json5" ]; then
                return 0
        else
                return 1
        fi
}

function build_sdk() {
	if [ -d ${ohos_sdk_path} ]; then
			echo "ohos-sdk exists."
			return 0
	fi
	SDK_PREBUILTS_PATH=${ROOT_PATH}/out/sdk/packages/ohos-sdk
	pushd ${ROOT_PATH}
		echo "building the latest ohos-sdk..."
		./build.py --product-name ohos-sdk --load-test-config=false --get-warning-list=false --stat-ccache=false --compute-overlap-rate=false --deps-guard=false --generate-ninja-trace=false --gn-args skip_generate_module_list_file=true sdk_platform=linux ndk_platform=linux use_cfi=false use_thin_lto=false enable_lto_O0=true sdk_check_flag=false enable_ndk_doxygen=false archive_ndk=false 
		if [[ "$?" -ne 0 ]]; then
		echo "ohos-sdk build failed! You can try to use '--no-prebuilt-sdk' to skip the build of ohos-sdk."
		exit 1
		fi
		if [ -d "${SDK_PREBUILTS_PATH}/linux" ]; then
			rm -rf ${SDK_PREBUILTS_PATH}/linux
		fi
		mkdir -p ${SDK_PREBUILTS_PATH}
		mv ${ROOT_PATH}/out/sdk/ohos-sdk/linux ${SDK_PREBUILTS_PATH}/
		mkdir -p ${SDK_PREBUILTS_PATH}/linux/native
		mv ${ROOT_PATH}/out/sdk/sdk-native/os-irrelevant/* ${SDK_PREBUILTS_PATH}/linux/native/
		mv ${ROOT_PATH}/out/sdk/sdk-native/os-specific/linux/* ${SDK_PREBUILTS_PATH}/linux/native/
		pushd ${SDK_PREBUILTS_PATH}/linux
		api_version=$(grep apiVersion toolchains/oh-uni-package.json | awk '{print $2}' | sed -r 's/\",?//g') || api_version="11"
		mkdir -p $api_version
		for i in */; do
			if [ -d "$i" ] && [ "$i" != "$api_version/" ]; then
				mv $i $api_version
			fi
		done
		popd
	popd
}

function parse_arguments() {
	local helperKey="";
	local helperValue="";
	local current="";

 	while [ "$1" != "" ]; do
		current=$1;
      		helperKey=${current#*--};
      		helperKey=${helperKey%%=*};
      		helperKey=$(echo "$helperKey" | tr '-' '_');
      		helperValue=${current#*=};
      		if [ "$helperValue" == "$current" ]; then
        		helperValue="1";
      		fi
      		#echo "eval arg_$helperKey=\"$helperValue\"";

      		eval "arg_$helperKey=\"$helperValue\"";
      		shift
  	done
}


parse_arguments "${@}";

if [ "$arg_help" != "0" ]; then
        print_help;
        exit 1;
fi

# Called in the warm-up process to ensure that the docker is the latest SDK every day
# Called like this: ./build.sh --build_sdk
if [ "$arg_build_sdk" == "1" ]; then
        rm -rf ${ohos_sdk_path}
        build_sdk
        exit 0;
fi

if [ "$arg_build_sdk" == "true" ]; then
	echo "start build sdk"
	build_sdk
	if [[ "$?" -ne 0 ]]; then
		echo "ohos-sdk build failed! You can try to use '--no-prebuilt-sdk' to skip the build of ohos-sdk."
		exit 1
	fi
	export OHOS_SDK_HOME=${ohos_sdk_path}
    echo "set OHOS_SDK_HOME to" ${OHOS_SDK_HOME}
fi


export PATH=/home/tools/command-line-tools/ohpm/bin:$PATH
npm config set registry https://repo.huaweicloud.com/repository/npm/
npm config set @ohos:registry https://repo.harmonyos.com/npm/
npm config set strict-ssl false
npm config set lockfile false
cat $HOME/.npmrc | grep 'lockfile=false' || echo 'lockfile=false' >> $HOME/.npmrc
if [ -d ${ROOT_PATH}/prebuilts/ohos-sdk/linux/10 ]; then
    mkdir -p ${ohos_sdk_path}
    mv -n ${ROOT_PATH}/prebuilts/ohos-sdk/linux/10 ${ohos_sdk_path}
    pushd ${ohos_sdk_path}
	sdk_version=$(grep version 10/toolchains/oh-uni-package.json | awk '{print $2}' | sed -r 's/\",?//g')
	mkdir -p ets
	ln -nsf ../10/ets ets/$sdk_version
	mkdir -p js
	ln -nsf ../10/js js/$sdk_version
	mkdir -p toolchains
	ln -nsf ../10/toolchains toolchains/$sdk_version
	mkdir -p native
	ln -nsf ../10/native native/$sdk_version
	mkdir -p previewer
	ln -nsf ../10/previewer previewer/$sdk_version
    popd
fi
if [ -d ${ROOT_PATH}/prebuilts/ohos-sdk/linux/11 ]; then
    mkdir -p ${ohos_sdk_path}
    mv -n ${ROOT_PATH}/prebuilts/ohos-sdk/linux/11 ${ohos_sdk_path}
    pushd ${ohos_sdk_path}
	sdk_version=$(grep version 11/toolchains/oh-uni-package.json | awk '{print $2}' | sed -r 's/\",?//g')
	mkdir -p ets
	ln -nsf ../11/ets ets/$sdk_version
	mkdir -p js
	ln -nsf ../11/js js/$sdk_version
	mkdir -p toolchains
	ln -nsf ../11/toolchains toolchains/$sdk_version
	mkdir -p native
	ln -nsf ../11/native native/$sdk_version
	mkdir -p previewer
	ln -nsf ../11/previewer previewer/$sdk_version
    popd
fi
if [ -d ${ROOT_PATH}/prebuilts/ohos-sdk/linux/12 ]; then
    mkdir -p ${ohos_sdk_path}
    mv -n ${ROOT_PATH}/prebuilts/ohos-sdk/linux/12 ${ohos_sdk_path}
    pushd ${ohos_sdk_path}
	sdk_version=$(grep version 12/toolchains/oh-uni-package.json | awk '{print $2}' | sed -r 's/\",?//g')
	mkdir -p ets
	ln -nsf ../12/ets ets/$sdk_version
	mkdir -p js
	ln -nsf ../12/js js/$sdk_version
	mkdir -p toolchains
	ln -nsf ../12/toolchains toolchains/$sdk_version
	mkdir -p native
	ln -nsf ../12/native native/$sdk_version
	mkdir -p previewer
	ln -nsf ../12/previewer previewer/$sdk_version
    popd
fi
if [ -d ${ROOT_PATH}/prebuilts/ohos-sdk/linux/14 ]; then
    mkdir -p ${ohos_sdk_path}
    mv -n ${ROOT_PATH}/prebuilts/ohos-sdk/linux/14 ${ohos_sdk_path}
    pushd ${ohos_sdk_path}
	sdk_version=$(grep version 14/toolchains/oh-uni-package.json | awk '{print $2}' | sed -r 's/\",?//g')
	mkdir -p ets
	ln -nsf ../14/ets ets/$sdk_version
	mkdir -p js
	ln -nsf ../14/js js/$sdk_version
	mkdir -p toolchains
	ln -nsf ../14/toolchains toolchains/$sdk_version
	mkdir -p native
	ln -nsf ../14/native native/$sdk_version
	mkdir -p previewer
	ln -nsf ../14/previewer previewer/$sdk_version
    popd
fi
if [ -d ${ROOT_PATH}/prebuilts/ohos-sdk/linux/18 ]; then
    mkdir -p ${ohos_sdk_path}
    mv -n ${ROOT_PATH}/prebuilts/ohos-sdk/linux/18 ${ohos_sdk_path}
    pushd ${ohos_sdk_path}
	sdk_version=$(grep version 18/toolchains/oh-uni-package.json | awk '{print $2}' | sed -r 's/\",?//g')
	mkdir -p ets
	ln -nsf ../18/ets ets/$sdk_version
	mkdir -p js
	ln -nsf ../18/js js/$sdk_version
	mkdir -p toolchains
	ln -nsf ../18/toolchains toolchains/$sdk_version
	mkdir -p native
	ln -nsf ../18/native native/$sdk_version
	mkdir -p previewer
	ln -nsf ../18/previewer previewer/$sdk_version
    popd
fi
if [ -d ${ROOT_PATH}/prebuilts/ohos-sdk/linux/19 ]; then
    mkdir -p ${ohos_sdk_path}
    mv -n ${ROOT_PATH}/prebuilts/ohos-sdk/linux/19 ${ohos_sdk_path}
    pushd ${ohos_sdk_path}
	sdk_version=$(grep version 19/toolchains/oh-uni-package.json | awk '{print $2}' | sed -r 's/\",?//g')
	mkdir -p ets
	ln -nsf ../19/ets ets/$sdk_version
	mkdir -p js
	ln -nsf ../19/js js/$sdk_version
	mkdir -p toolchains
	ln -nsf ../19/toolchains toolchains/$sdk_version
	mkdir -p native
	ln -nsf ../19/native native/$sdk_version
	mkdir -p previewer
	ln -nsf ../19/previewer previewer/$sdk_version
    popd
fi
if [ -d ${ROOT_PATH}/prebuilts/ohos-sdk/linux/20 ]; then
    mkdir -p ${ohos_sdk_path}
    mv -n ${ROOT_PATH}/prebuilts/ohos-sdk/linux/20 ${ohos_sdk_path}
    pushd ${ohos_sdk_path}
	sdk_version=$(grep version 20/toolchains/oh-uni-package.json | awk '{print $2}' | sed -r 's/\",?//g')
	mkdir -p ets
	ln -nsf ../20/ets ets/$sdk_version
	mkdir -p js
	ln -nsf ../20/js js/$sdk_version
	mkdir -p toolchains
	ln -nsf ../20/toolchains toolchains/$sdk_version
	mkdir -p native
	ln -nsf ../20/native native/$sdk_version
	mkdir -p previewer
	ln -nsf ../20/previewer previewer/$sdk_version
    popd
fi
if [ "${arg_project}" == "" -a "${arg_url}" == "" ]; then
        echo "--project or --url is not null"
        exit 1;
fi


if [ ! -d "${arg_project}" ]; then
        echo "${arg_project} is not dir"
        exit 1;
fi


if [[ ${arg_project} = */ ]]; then
	arg_project=${arg_project%/}
fi


if [[ ${arg_sign_tool} = */ ]]; then
        arg_sign_tool=${arg_sign_tool%/}
fi

if [[ ${arg_p7b} = "" ]]; then
        arg_p7b=$(find ${arg_project} -name *.p7b | head -n 1)
        if [[ ${arg_p7b} = "" ]]; then
                arg_p7b=openharmony_sx.p7b
        fi
fi

clear_dir ${arg_out_path}


if [ "${arg_url}" != "" ]; then
	if [ "${arg_branch}" == "" ]; then
		echo "branch is not null"
		exit 1
	fi
	project_name=${arg_url##*/}
	project_name=${project_name%%.git*}
        if test -d ${BASE_PATH}/projects/${project_name}
        then
                echo "${project_name} exists,ready for update..."
                cd ${BASE_PATH}/projects/${project_name}
                git fetch origin ${arg_branch}
                git reset --hard origin/${arg_branch}
                git pull
        else
                echo "${project_name} dose not exist,ready to download..."
                cd ${BASE_PATH}/projects
                git clone -b ${arg_branch} ${arg_url} ${project_name}
        fi
	arg_project=${BASE_PATH}/projects/${project_name}
fi


if ! is_project_root ${arg_project}; then
        echo "${arg_project} is not OpenHarmony Project"
        exit 1;
fi

if [ "${arg_sdk_path}" != "" ]; then
	export OHOS_SDK_HOME=${arg_sdk_path}
fi
export OHOS_BASE_SDK_HOME=${OHOS_SDK_HOME}

echo "start build hap..."
cd ${arg_project}
echo "sdk.dir=${OHOS_SDK_HOME}"  > ./local.properties
echo "nodejs.dir=${NODE_HOME}" >> ./local.properties

echo "use sdk:"${OHOS_SDK_HOME}

is_ohpm=true
package_json_name="oh-package.json5"
if [ ! -f ${arg_project}/${package_json_name} ]; then
	is_ohpm=false
	package_json_name="package.json"
fi

if ${is_ohpm}; then
	if [ "${arg_ohpm}" == "" ]; then
		arg_ohpm="@ohos:registry https://ohpm.openharmony.cn/ohpm/"
	fi
	echo "ohpm config set ${arg_ohpm}"
	ohpm config set ${arg_ohpm}	
else
	if [ "${arg_npm}" == "" ]; then
		arg_npm="@ohos:registry=https://repo.harmonyos.com/npm/"
	fi
	echo "npm config set ${arg_npm}"
	npm config set ${arg_npm}
fi

module_list=()
module_name=()
out_module=()
bundle_name=""


function del_module_name(){
        name=${1}
        for i in "${!module_name[@]}"
        do
                if [[ "${module_name[i]}" == "${name}" ]]; then
                        unset module_name[i]
			echo "移除"${name}" , 剩余 : "${module_name[@]}
			return 0
                fi
        done
        return 1
}


function load_dep(){
	local cur_m_n=${1}
	local cur_module
	for cur_module in ${module_list[@]}
	do
		if [[ "${cur_module}" =~ "${cur_m_n}" ]]; then
			del_module_name ${cur_m_n}
			for m_n_1 in ${module_name[@]}
			do
				rr=$(cat ${cur_module}"/${package_json_name}" | grep "${m_n_1}" || true)
				if [[ "${rr}" != "" ]]; then
					load_dep ${m_n_1}
				fi
			done
			cd ${cur_module}
			echo ${cur_module}" 执行npm/ohpm install"
			if ${is_ohpm}; then
				ohpm install
			else
				npm i
			fi

		fi
	done
}


while read line
do
	if [[ ${line} =~ "srcPath" ]]; then
		pa=${line%\"*}
		pa=${pa##*\".}
		module_list[${#module_list[*]}]=${arg_project}${pa}
		module_name[${#module_name[*]}]=${pa}
		if [ -d "${arg_project}/AppScope" ]; then
			cur_bundle_line=`cat ${arg_project}/AppScope/app.json5 | grep "\"bundleName\""`
			bundle_name=${cur_bundle_line%\"*}
			bundle_name=${bundle_name##*\"}
			# echo "bundleName : "${bundle_name}
			is_entry=`cat ${arg_project}${pa}/src/main/module.json5 | sed 's/ //g' | grep "\"type\":\"entry\"" || true`
			is_feature=`cat ${arg_project}${pa}/src/main/module.json5 | sed 's/ //g' | grep "\"type\":\"feature\"" || true`
			if [[ "${is_entry}" != "" || "${is_feature}" != "" ]]; then
				echo "hap输出module: "${arg_project}${pa}
				out_module[${#out_module[*]}]=${arg_project}${pa}
			fi
		else
			cur_bundle_line=`cat ${arg_project}${pa}/src/main/config.json | grep "\"bundleName\""`
                        bundle_name=${cur_bundle_line%\"*}
                        bundle_name=${bundle_name##*\"}
                        # echo "bundleName : "${bundle_name}
			is_entry=`cat ${arg_project}${pa}/src/main/config.json | sed 's/ //g' | grep "\"moduleType\":\"entry\"" || true`
                        is_feature=`cat ${arg_project}${pa}/src/main/config.json | sed 's/ //g' | grep "\"moduleType\":\"feature\"" || true`
                        if [[ "${is_entry}" != "" || "${is_feature}" != "" ]]; then
                                echo "hap输出module: "${arg_project}${pa}
                                out_module[${#out_module[*]}]=${arg_project}${pa}
                        fi
		fi
	fi
done < ${arg_project}"/build-profile.json5"


for module in ${module_list[@]}
do
	if del_module_name ${module##${arg_project}}; then
		for m_n in ${module_name[@]}
		do
			rr=$(cat ${module}"/${package_json_name}" | grep "${m_n}" || true)
			if [[ "${rr}" != "" ]]; then
				load_dep ${m_n}
			fi
		done
		cd ${module}
		echo ${module}" 执行npm/ohpm install"
		if ${is_ohpm}; then
			ohpm install
		else
			npm i
		fi
	fi	
done


cd ${arg_project}
echo ${arg_project}" 执行npm/ohpm install"
if ${is_ohpm}; then
        ohpm install
        if [ ! -f "hvigorw" ]; then
                hvigorw clean --no-daemon
                hvigorw assembleHap --mode module -p product=default -p debuggable=false --no-daemon
        else
                chmod +x hvigorw
                # Historical reasons need to be compatible with NODE_HOME path issue
                if grep -q "\${NODE_HOME}/bin/node" hvigorw ; then
                # node home path include "bin"
                        if [ ! -x "${NODE_HOME}/bin/node" ];then
                        export NODE_HOME=$(dirname ${NODE_HOME})
                        fi
                else
                # node home path does not include "bin"
                        if [ -x "${NODE_HOME}/bin/node" ];then
                        export NODE_HOME=${NODE_HOME}/bin
                        fi
                fi
                ./hvigorw clean --no-daemon
                ./hvigorw assembleHap --mode module -p product=default -p debuggable=false --no-daemon
        fi
else
        npm install
        node ./node_modules/@ohos/hvigor/bin/hvigor.js clean
        node ./node_modules/@ohos/hvigor/bin/hvigor.js --mode module clean assembleHap -p debuggable=false
fi



for module in ${out_module[@]}
do
	cur_out_module_name=${module##*/}
	is_sign=false
	echo "module = ${module} , cur_out_module_name=${cur_out_module_name}"
	if [ ! -d ${module}/build/default/outputs/default/ ]; then
		echo "module = ${module}, assembleHap error !!!"
		continue
	fi
	for out_file in `ls ${module}/build/default/outputs/default/`
	do
		if [[ "${out_file}" =~ "-signed.hap" ]]; then
			is_sign=true
			echo "发现signed包 : "${out_file}",直接归档"
			cp ${module}/build/default/outputs/default/${out_file} ${arg_out_path}/
			break
		fi
	done
	if test ${is_sign} = false
	then
		hap_name=${arg_project##*/}
		# echo "${hap_name},skip sign 'hap'. Invalid signingConfig is configured for 'default' product."
		for out_file in `ls ${module}/build/default/outputs/default/`
        	do
                	if [[ "${out_file}" =~ "-unsigned.hap" ]]; then
                        	echo "发现unsigned包 : "${out_file}",开始使用签名工具签名"
				nosign_hap_path=${module}/build/default/outputs/default/${out_file}
				sign_hap_path=${module}/build/default/outputs/default/${out_file/unsigned/signed}
				cp -r ${arg_sign_tool} ${arg_project}/
        			cd ${arg_project}/dist
				if [ ! -e ${arg_profile} ]; then
					echo "${arg_profile} not exist! ! !"
					exit 1
				fi
				if [ "${arg_bundle_name}" != "" ]; then
					sed -i "s/\"com.OpenHarmony.app.test\"/\"${arg_bundle_name}\"/g" ${arg_profile}
				else
					sed -i "s/\"com.OpenHarmony.app.test\"/\"${bundle_name}\"/g" ${arg_profile}
				fi
       		 		sed -i "s/\"normal\"/\"${arg_apl}\"/g" ${arg_profile}
        			sed -i "s/\"system_basic\"/\"${arg_apl}\"/g" ${arg_profile}
        			sed -i "s/\"system_core\"/\"${arg_apl}\"/g" ${arg_profile}
        			sed -i "s/\"hos_normal_app\"/\"${arg_feature}\"/g" ${arg_profile}
        			sed -i "s/\"hos_system_app\"/\"${arg_feature}\"/g" ${arg_profile}
        			java -jar hap-sign-tool.jar  sign-profile -keyAlias "openharmony application profile release" -signAlg "SHA256withECDSA" -mode "localSign" -profileCertFile "OpenHarmonyProfileRelease.pem" -inFile "${arg_profile}" -keystoreFile "OpenHarmony.p12" -outFile "openharmony_sx.p7b" -keyPwd "123456" -keystorePwd "123456"
        			java -jar hap-sign-tool.jar sign-app -keyAlias "openharmony application release" -signAlg "SHA256withECDSA" -mode "localSign" -appCertFile "OpenHarmonyApplication.pem" -profileFile "${arg_p7b}" -inFile "${nosign_hap_path}" -keystoreFile "OpenHarmony.p12" -outFile "${sign_hap_path}" -keyPwd "123456" -keystorePwd "123456"
        			cp ${sign_hap_path} ${arg_out_path}/
				is_sign=true
                	fi
        	done
		if test ${is_sign} = false
		then
                	echo "${module} assembleHap error !!!"
                	rm -rf ${arg_project}/sign_helper
                	exit 1
        	fi
	fi
done
rm -rf ${arg_project}/sign_helper

# backup sourceMaps.json files for DFR
cd ${arg_project}
find . -name "sourceMaps.json" -type f | while read file; do
    sourceMaps_path=$(echo ${file} | sed 's/^\.\///;s/\/sourceMaps.json$//')
    mkdir -p ${arg_out_path}/${sourceMaps_path}
    cp ${file} ${arg_out_path}/${sourceMaps_path}/
done

exit 0
