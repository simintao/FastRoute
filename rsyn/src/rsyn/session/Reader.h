/* Copyright 2014-2017 Rsyn
 *
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
 
/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Reader.h
 * Author: jucemar
 *
 * Created on 18 de Fevereiro de 2017, 08:20
 */

#ifndef RSYN_READER_H
#define RSYN_READER_H

#include "rsyn/util/Json.h"
#include "rsyn/io/reader/PopulateRsyn.h"

namespace Rsyn {

class Session;


class Reader : public PopulateRsyn {
public:
	virtual bool load(const Rsyn::Json &params) = 0;
}; // end class

} // end namespace


#endif /* RSYN_READER_H */

