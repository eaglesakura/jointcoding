/*
 * jcViewOnClickListener.h
 *
 *  Created on: 2012/10/06
 */

#ifndef JCVIEWONCLICKLISTENER_H_
#define JCVIEWONCLICKLISTENER_H_

namespace jc {

/**
 * Viewがクリックされた。
 */
class ViewOnClickListener {
public:
    virtual ~ViewOnClickListener() {
    }

    /**
     * Viewがクリックされた。
     */
    virtual void onClick() = 0;
};

}

#endif /* JCVIEWONCLICKLISTENER_H_ */
