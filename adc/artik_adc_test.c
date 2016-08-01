
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <artik_module.h>
#include <artik_adc.h>

static artik_adc_config config = { 0, "adc", NULL };

artik_error adc_test_value(void)
{
	artik_adc_handle handle;
	artik_error ret = S_OK;
	artik_adc_module *adc = (artik_adc_module *)artik_request_api_module("adc");
	int val = -1;
	int i = 0;

	fprintf(stdout, "TEST: %s\n", __func__);
	if (adc == INVALID_MODULE) {
		fprintf(stderr, "TEST: %s - Failed to request module\n", __func__);
		return E_NOT_SUPPORTED;
	}

	ret = adc->request(&handle, &config);
	if (ret != S_OK) {
		fprintf(stderr, "TEST: %s - Failed to request adc (err=%d)\n", __func__, ret);
		goto exit;
	}

	for (i = 0; i < 5; i++) {
		ret = adc->get_value(handle, &val);
		if (ret != S_OK) {
			fprintf(stderr, "TEST: %s - Failed to get adc value (err=%d)\n", __func__, ret);
			goto exit;
		}
		fprintf(stdout, "TEST: %s - Value = %d\n", __func__, val);
	    usleep(1000*1000);
	}

	adc->release(handle);

exit:
	fprintf(stdout, "TEST: %s %s\n", __func__, (ret == S_OK) ? "succeeded" : "failed");

	ret = artik_release_api_module(adc);
	if (ret != S_OK) {
		fprintf(stderr, "TEST: %s - Failed to release module\n", __func__);
		return ret;
	}

	return S_OK;
}

int main(void)
{
	artik_error ret = S_OK;

	ret = adc_test_value();

	return (ret == S_OK) ? 0 : -1;
}
