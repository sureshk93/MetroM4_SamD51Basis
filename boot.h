/*
 Copyright 2019 June Hanabi

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/

/*
 * boot.h
 *
 * Created: 9/26/2019 11:39:03 PM
 *  Author: juneh
 */


#ifndef BOOT_H_
#define BOOT_H_

#include "sam.h"

class Boot
{
public:
  void static __attribute__((cold)) boot();
};

#endif /* BOOT_H_ */
