//
//  UTTypeAdapter.h
//  CupsUtils
//
//  Created by Danil Korotenko on 5/7/21.
//

#import <UniformTypeIdentifiers/UTType.h>

#include <string>

std::string UTFileExtensionForMIMEType(const std::string &aMIMEType)
{
    NSString *mime = [NSString stringWithUTF8String:aMIMEType.c_str()];

    UTType *type = [UTType typeWithMIMEType:mime];

    NSString *extension = [type preferredFilenameExtension];

    std::string result([extension UTF8String], [extension length]);

    return result;
}

