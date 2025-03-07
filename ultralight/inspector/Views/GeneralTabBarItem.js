/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
 * Copyright (C) 2016 Devin Rousso <webkit@devinrousso.com>. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

WI.GeneralTabBarItem = class GeneralTabBarItem extends WI.TabBarItem
{
    // Static

    static fromTabContentView(tabContentView)
    {
        console.assert(tabContentView instanceof WI.TabContentView);

        let {image, displayName, title} = tabContentView.tabInfo();
        return new WI.GeneralTabBarItem(tabContentView, image, displayName, title);
    }

    // Public

    get displayName()
    {
        return super.displayName;
    }

    set displayName(displayName)
    {
        if (displayName) {
            this._displayNameElement = document.createElement("span");
            this._displayNameElement.className = "name";

            let displayNameContentElement = this._displayNameElement.appendChild(document.createElement("span"));
            displayNameContentElement.className = "content";
            displayNameContentElement.textContent = displayName;

            this.element.insertBefore(this._displayNameElement, this.element.lastChild);
        } else {
            if (this._displayNameElement)
                this._displayNameElement.remove();

            this._displayNameElement = null;
        }

        super.displayName = displayName;
    }
};
