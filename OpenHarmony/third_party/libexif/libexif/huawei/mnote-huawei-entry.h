/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LIBEXIF_MNOTE_HUAWEI_ENTRY_H
#define LIBEXIF_MNOTE_HUAWEI_ENTRY_H

#include <libexif/exif-format.h>
#include <libexif/exif-byte-order.h>
#include <libexif/huawei/mnote-huawei-tag.h>

typedef struct _MnoteHuaweiEntry        MnoteHuaweiEntry;
typedef struct _ExifMnoteDataHuawei     ExifMnoteDataHuawei;

struct _MnoteHuaweiEntry {
	MnoteHuaweiTag tag;
	ExifFormat format;
	unsigned long components;

	unsigned char *data;
	unsigned int size;
	ExifByteOrder order;

	void *md;
	void *parent_md;
	ExifMem *mem;
};

/*
 * This structure is used to store the initialization information for each tag.
 * It is used to initialize the tag when the tag is created.
 */
typedef struct {
	/*
	 * Huawei custom tag type
	 */
	MnoteHuaweiTag tag;

	/*
	 * Exif format type
	 */
	ExifFormat format;

	/*
	 * Number of components
	 * For example, the number of components for the tag MNOTE_HUAWEI_SCENE_INFO is 1.
	 */
	unsigned int components;

	/*
	 * Default ASCII value
	 * For example, the default ASCII value for the tag MNOTE_HUAWEI_SCENE_INFO is "[None]".
	 */
	const char *default_ascii;

	/*
	 * Number of rational components
	 * false is 0, true is 1
	 */
	int zero_initialize;
} HuaweiTagInitInfo;

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

char *mnote_huawei_entry_get_value (MnoteHuaweiEntry *entry, char *v, unsigned int maxlen);
int mnote_huawei_entry_set_value(MnoteHuaweiEntry *entry, const char *v, int strlen);

MnoteHuaweiEntry *mnote_huawei_entry_new(ExifMnoteData *n);
void mnote_huawei_entry_replace_mem(MnoteHuaweiEntry *e, ExifMem *mem);
void mnote_huawei_entry_free_content(MnoteHuaweiEntry *e);
void mnote_huawei_entry_free_contour(MnoteHuaweiEntry *e);
void mnote_huawei_entry_free(MnoteHuaweiEntry *e);
void mnote_huawei_entry_initialize(MnoteHuaweiEntry *e, MnoteHuaweiTag tag, ExifByteOrder order);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !defined(LIBEXIF_MNOTE_HUAWEI_ENTRY_H) */
