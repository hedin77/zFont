// Supported with union (c) 2020 Union team
// Union SOURCE file

namespace GOTHIC_ENGINE {
    int BilerpFiltering;
    int zBufferWriting;
    zTRnd_ZBufferCmp zBufferComparing;

    void PushPrintRenderOptions(int bilertFlt, int zbufWrt, zTRnd_ZBufferCmp zbufCmp) {
        BilerpFiltering = zrenderer->GetBilerpFilterEnabled();
        zBufferWriting = zrenderer->GetZBufferWriteEnabled();
        zBufferComparing = zrenderer->GetZBufferCompare();
        auto AlphaBlendingFunc = zrenderer->GetAlphaBlendFunc();

        zrenderer->SetBilerpFilterEnabled(bilertFlt);
        zrenderer->SetZBufferWriteEnabled(zbufWrt);
        zrenderer->SetZBufferCompare(zbufCmp);

        if ((AlphaBlendingFunc != zRND_ALPHA_FUNC_ADD) && (AlphaBlendingFunc != zRND_ALPHA_FUNC_BLEND))
            zrenderer->SetAlphaBlendFunc(zRND_ALPHA_FUNC_BLEND);
    }

    void PopPrintRenderOptions() {
        zrenderer->SetBilerpFilterEnabled(BilerpFiltering);
        zrenderer->SetZBufferWriteEnabled(zBufferWriting);
        zrenderer->SetZBufferCompare(zBufferComparing);
    }


    float GetCameraFarZ() {
#if ENGINE >= Engine_G2
        return zCCamera::activeCam ? zCCamera::activeCam->nearClipZ + 1.0f : 1.0f;
#else
        return 1.0f;
#endif
    }


    inline Array<zSTRING> ViewSplitLines(zSTRING text, int viewWidthPx, Font* ttf) {
        Array<zSTRING> lines;

        std::u32string unicode;
        charPtr_2_utf32(text.ToChar(), unicode);

        int lineStart = 0;
        int lineWidth = 0;
        int lastSpacePos = -1;

        for (int i = 0; i < unicode.length(); i++) {
            char32_t u32 = unicode[i];
            int letterWidth = ttf->GetLetterWidth(u32);
            if (u32 == ' ' || u32 == '\t') {
                lastSpacePos = i;
            }
            if (u32 == '\n') {
                std::u32string substr = unicode.substr(lineStart, i - lineStart);
                zSTRING charPtr;
                utf32_2_charPtr(substr, charPtr);
                lines.Insert(charPtr);

                lineStart = i + 1;
                lineWidth = 0;
                lastSpacePos = -1;
                continue;
            }
            if (lineWidth + letterWidth > viewWidthPx) {
                int breakPos;
                if (lastSpacePos >= lineStart) {
                    breakPos = lastSpacePos;
                }
                else {
                    breakPos = i - 1;
                    if (breakPos < lineStart) {
                        breakPos = lineStart; 
                    }
                }
                std::u32string substr = unicode.substr(lineStart, breakPos - lineStart);
                zSTRING charPtr;
                utf32_2_charPtr(substr, charPtr);
                lines.Insert(charPtr);
                if (lastSpacePos >= lineStart) {
                    lineStart = lastSpacePos + 1;
                }
                else {
                    lineStart = breakPos;
                }
                lineWidth = 0;
                for (int j = lineStart; j <= i; j++) {
                    lineWidth += ttf->GetLetterWidth(unicode[j]);
                }
                lastSpacePos = -1;
            }
            else {
                lineWidth += letterWidth;
            }
        }

        if (lineStart < unicode.length()) {
            std::u32string substr = unicode.substr(lineStart);
            zSTRING charPtr;
            utf32_2_charPtr(substr, charPtr);
            lines.Insert(charPtr);
        }

        return lines;
    }
}