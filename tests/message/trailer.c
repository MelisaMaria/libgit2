#include "clar_libgit2.h"
#include "message.h"

struct trailer {
	const char *key;
	const char *value;
};

struct cb_state {
	struct trailer *trailer;
};

static int trailer_cb(const char *key, const char *value, void *st_)
{
	struct cb_state *st = st_;

	cl_assert_equal_s(st->trailer->key, key);
	cl_assert_equal_s(st->trailer->value, value);

	st->trailer++;

	return 0;
}

static void assert_trailers(const char *message, struct trailer *trailers)
{
	struct cb_state st = { trailers };

	int rc = git_message_trailers(message, trailer_cb, &st);

	cl_assert_equal_s(NULL, st.trailer->key);
	cl_assert_equal_s(NULL, st.trailer->value);

	cl_assert_equal_i(0, rc);
}

void test_message_trailer__simple(void)
{
	struct trailer trailers[3];

	trailers[0].key   = "Signed-off-by";
	trailers[0].value = "foo@bar.com";
	trailers[1].key   = "Signed-off-by";
	trailers[1].value = "someone@else.com";
	trailers[2].key   = NULL;
	trailers[2].value = NULL;

	assert_trailers(
		"Message\n"
		"\n"
		"Signed-off-by: foo@bar.com\n"
		"Signed-off-by: someone@else.com\n"
	, trailers);
}

void test_message_trailer__no_whitespace(void)
{
	struct trailer trailers[2];

	trailers[0].key   = "Key";
	trailers[0].value = "value";
	trailers[1].key   = NULL;
	trailers[1].value = NULL;

	assert_trailers(
		"Message\n"
		"\n"
		"Key:value\n"
	, trailers);
}

void test_message_trailer__extra_whitespace(void)
{
	struct trailer trailers[2];

	trailers[0].key   = "Key";
	trailers[0].value = "value";
	trailers[1].key   = NULL;
	trailers[1].value = NULL;

	assert_trailers(
		"Message\n"
		"\n"
		"Key   :   value\n"
	, trailers);
}

void test_message_trailer__no_newline(void)
{
	struct trailer trailers[2];

	trailers[0].key   = "Key";
	trailers[0].value = "value";
	trailers[1].key   = NULL;
	trailers[1].value = NULL;

	assert_trailers(
		"Message\n"
		"\n"
		"Key: value"
	, trailers);
}

void test_message_trailer__not_last_paragraph(void)
{
	struct trailer trailers;

	trailers.key   = NULL;
	trailers.value = NULL;

	assert_trailers(
		"Message\n"
		"\n"
		"Key: value\n"
		"\n"
		"More stuff\n"
	, &trailers);
}

void test_message_trailer__conflicts(void)
{
	struct trailer trailers[2];

	trailers[0].key   = "Key";
	trailers[0].value = "value";
	trailers[1].key   = NULL;
	trailers[1].value = NULL;

	assert_trailers(
		"Message\n"
		"\n"
		"Key: value\n"
		"\n"
		"Conflicts:\n"
		"\tfoo.c\n"
	, trailers);
}

void test_message_trailer__patch(void)
{
	struct trailer trailers[2];

	trailers[0].key   = "Key";
	trailers[0].value = "value";
	trailers[1].key   = NULL;
	trailers[1].value = NULL;

	assert_trailers(
		"Message\n"
		"\n"
		"Key: value\n"
		"\n"
		"---\n"
		"More: stuff\n"
	, trailers);
}

void test_message_trailer__continuation(void)
{
	struct trailer trailers[4];

	trailers[0].key   = "A";
	trailers[0].value = "b\n c";
	trailers[1].key   = "D";
	trailers[1].value = "e\n f: g h";
	trailers[2].key   = "I";
	trailers[2].value = "j";
	trailers[3].key   = NULL;
	trailers[3].value = NULL;

	assert_trailers(
		"Message\n"
		"\n"
		"A: b\n"
		" c\n"
		"D: e\n"
		" f: g h\n"
		"I: j\n"
	, trailers);
}

void test_message_trailer__invalid(void)
{
	struct trailer trailers[3];

	trailers[0].key   = "Signed-off-by";
	trailers[0].value = "some@one.com";
	trailers[1].key   = "Another";
	trailers[1].value = "trailer";
	trailers[2].key   = NULL;
	trailers[2].value = NULL;

	assert_trailers(
		"Message\n"
		"\n"
		"Signed-off-by: some@one.com\n"
		"Not a trailer\n"
		"Another: trailer\n"
	, trailers);
}