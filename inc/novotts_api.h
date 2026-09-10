
#ifndef NAVO_TTS_H_
#define NAVO_TTS_H_

#ifdef __cplusplus
extern "C" {
#endif


/**
 * @brief   onnxruntime 初始化。
 * @param   config[in]     配置目录，例子：./config/tts
 * @return  int            成功返回0，失败返回 1
 */
int novottsOnnxInit(const char *config);

/**
 * @brief   释放onnxruntime 占用的资源。
 */
void novottsOnnxRelease();

/**
 * @brief   novo tts 引擎初始化。
 * @param   handler[out]   novo tts 引擎句柄。
 * @param   config[in]     配置目录，同novottsOnnxInit，例子：./config/tts
 * @param   spkear[in]     发言人选择，norna norman
 * @return  int            成功返回0，失败返回 1
 */
int novottsEngineInit(void **handler, const char *config, const char *spkear);

/**
 * @brief   novo tts 根据文本生成音频。同步函数。50个单词内速度很快，如果文本太长则延迟会高，建议将文本分段送入引擎。
 * @param   handler[in]      novo tts 引擎句柄。
 * @param   text[in]         输入的文本，utf-8字符串
 * @param   audiodata[out]   输出audio buffer
 * @param   audiosize[out]   输出audio buffer size
 * @return  int              成功返回0，失败返回 1
 */
int novottsSynthesize(void *handler, const char *text, unsigned short **audiodata, int *audiosize);

/**
 * @brief   释放audio buffer内存。
 * @param   audiodata[in]  novo tts 引擎句柄。
 * @return  int            成功返回0，失败返回 1
 */
int novottsReturn(unsigned short *audiodata);


/**
 * @brief   novo tts 引擎释放。
 * @param   handler[in]   novo tts 引擎句柄。
 * @return  int            成功返回0，失败返回 1
 */
int novottsEngineUninit(void *handler);

#ifdef __cplusplus
}
#endif

#endif
