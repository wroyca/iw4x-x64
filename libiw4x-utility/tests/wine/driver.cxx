#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#include <libiw4x/utility/wine.hxx>

#undef NDEBUG
#include <cassert>

using namespace std;
using namespace iw4x::utility;

static void
test_version ()
{
  // Basic versions
  //
  {
    auto v (wine_parse_version ("9.0"));
    assert (v);
    assert (v->major == 9 && v->minor == 0 && v->patch == 0);
    assert (v->rc == 0);
    assert (v->build_info.empty ());
    assert (v->to_string() == "9.0");
  }

  {
    auto v (wine_parse_version ("8.21"));
    assert (v);
    assert (v->major == 8 && v->minor == 21 && v->patch == 0);
    assert (v->rc == 0);
    assert (v->build_info.empty ());
    assert (v->to_string() == "8.21");
  }

  // Version with patch
  //
  {
    auto v (wine_parse_version ("8.0.2"));
    assert (v);
    assert (v->major == 8 && v->minor == 0 && v->patch == 2);
    assert (v->rc == 0);
    assert (v->build_info.empty ());
    assert (v->to_string() == "8.0.2");
  }

  // RC versions
  //
  {
    auto v (wine_parse_version ("9.0-rc1"));
    assert (v);
    assert (v->major == 9 && v->minor == 0 && v->patch == 0);
    assert (v->rc == 1);
    assert (v->build_info.empty ());
    assert (v->to_string() == "9.0-rc1");
  }

  {
    auto v (wine_parse_version ("9.0-rc10"));
    assert (v);
    assert (v->major == 9 && v->minor == 0 && v->patch == 0);
    assert (v->rc == 10);
    assert (v->build_info.empty ());
    assert (v->to_string() == "9.0-rc10");
  }

  // Versions with build info
  //
  {
    auto v (wine_parse_version ("wine-9.0 (Staging)"));
    assert (v);
    assert (v->major == 9 && v->minor == 0 && v->patch == 0);
    assert (v->rc == 0);
    assert (v->build_info == "(Staging)");
    assert (v->to_string() == "9.0 (Staging)");
  }

  {
    auto v (wine_parse_version ("9.0 (Staging)"));
    assert (v);
    assert (v->major == 9 && v->minor == 0 && v->patch == 0);
    assert (v->rc == 0);
    assert (v->build_info == "(Staging)");
    assert (v->to_string() == "9.0 (Staging)");
  }

  {
    auto v (wine_parse_version ("9.0-rc1 (Staging)"));
    assert (v);
    assert (v->major == 9 && v->minor == 0 && v->patch == 0);
    assert (v->rc == 1);
    assert (v->build_info == "(Staging)");
    assert (v->to_string() == "9.0-rc1 (Staging)");
  }

  {
    auto v (wine_parse_version ("8.0.2 (Ubuntu 22.04)"));
    assert (v);
    assert (v->major == 8 && v->minor == 0 && v->patch == 2);
    assert (v->rc == 0);
    assert (v->build_info == "(Ubuntu 22.04)");
    assert (v->to_string() == "8.0.2 (Ubuntu 22.04)");
  }

  // wine- prefix variations
  //
  {
    auto v (wine_parse_version ("wine-8.21"));
    assert (v);
    assert (v->major == 8 && v->minor == 21 && v->patch == 0);
    assert (v->rc == 0);
    assert (v->build_info.empty ());
    assert (v->to_string() == "8.21");
  }

  // Invalid versions
  //
  {
    auto r1 (wine_parse_version (""));
    assert (!r1);
    assert (r1.error ().what () == string ("invalid major version"));

    auto r2 (wine_parse_version ("9"));
    assert (!r2);
    assert (r2.error ().what () == string ("missing or invalid minor version"));

    auto r3 (wine_parse_version ("9."));
    assert (!r3);
    assert (r3.error ().what () == string ("invalid minor version"));

    auto r4 (wine_parse_version (".0"));
    assert (!r4);
    assert (r4.error ().what () == string ("invalid major version"));

    auto r5 (wine_parse_version ("9.0."));
    assert (!r5);
    assert (r5.error ().what () == string ("invalid patch version"));

    auto r6 (wine_parse_version ("9.0-"));
    assert (!r6);
    assert (r6.error ().what () == string ("invalid RC format"));

    auto r7 (wine_parse_version ("9.0-rc"));
    assert (!r7);
    assert (r7.error ().what () == string ("invalid RC version"));

    auto r8 (wine_parse_version ("9.0-beta1"));
    assert (!r8);
    assert (r8.error ().what () == string ("invalid RC format"));

    auto r9 (wine_parse_version ("a.b"));
    assert (!r9);
    assert (r9.error ().what () == string ("invalid major version"));

    auto r10 (wine_parse_version ("9.0a"));
    assert (!r10);
    assert (r10.error ().what () == string ("invalid build info format"));

    auto r11 (wine_parse_version ("wine"));
    assert (!r11);
    assert (r11.error ().what () == string ("invalid major version"));

    auto r12 (wine_parse_version ("wine-"));
    assert (!r12);
    assert (r12.error ().what () == string ("invalid major version"));

    auto r13 (wine_parse_version ("wine-a.b"));
    assert (!r13);
    assert (r13.error ().what () == string ("invalid major version"));
  }

  // Edge cases with whitespace
  //
  {
    auto v (wine_parse_version ("9.0  (Staging)"));
    assert (v);
    assert (v->build_info == "(Staging)");
  }

  {
    auto v (wine_parse_version ("9.0    Multiple   Spaces"));
    assert (v);
    assert (v->build_info == "Multiple   Spaces");
  }
}

static void
test_version_comparison ()
{
  wine_version v1 ("9.0");
  wine_version v2 ("8.21");
  wine_version v3 ("9.0");
  wine_version v4 ("9.0.1");
  wine_version v5 ("9.0-rc1");

  // Major version comparison
  //
  assert (v1 > v2);
  assert (v2 < v1);

  // Equal versions
  //
  assert (v1 == v3);

  // Patch version comparison
  //
  assert (v4 > v1);
  assert (v1 < v4);

  // RC vs stable: RC is less than stable
  //
  assert (v5 < v1);
  assert (v1 > v5);

  // Minor version
  //
  wine_version v6 ("9.1");
  assert (v6 > v1);
  assert (v1 < v6);

  // RC comparison
  //
  wine_version v7 ("9.0-rc2");
  assert (v7 > v5);
  assert (v5 < v7);
}

static void
test_version_constructor ()
{
  // String constructor with valid input
  //
  {
    wine_version v ("9.0");
    assert (v.major == 9 && v.minor == 0 && v.patch == 0);
  }

  // String constructor with invalid input
  //
  {
    try
    {
      wine_version v ("invalid");
      assert (false);
    }
    catch (const invalid_argument& e)
    {
      assert (string (e.what ()).find ("invalid wine version") != string::npos);
    }
  }

  // Test versions with build info
  //
  {
    wine_version v ("8.21.2 Ubuntu");
    assert (v.major == 8);
    assert (v.minor == 21);
    assert (v.patch == 2);
    assert (v.rc == 0);
    assert (v.build_info == "Ubuntu");
    assert (v.to_string() == "8.21.2 Ubuntu");
  }

  {
    wine_version v ("9.0-rc1 Staging");
    assert (v.major == 9);
    assert (v.minor == 0);
    assert (v.patch == 0);
    assert (v.rc == 1);
    assert (v.build_info == "Staging");
    assert (v.to_string() == "9.0-rc1 Staging");
  }
}

static void
test_version_roundtrip ()
{
  // Test that parsing and string conversion are inverse operations
  //
  vector<string> versions = {
    "9.0",
    "8.21",
    "8.0.2",
    "9.0-rc1",
    "9.0-rc10",
    "9.0 (Staging)",
    "9.0-rc1 (Staging)",
    "8.0.2 (Ubuntu 22.04)"
  };

  for (const auto& ver_str : versions)
  {
    auto v (wine_parse_version (ver_str));
    assert (v);

    // For versions with wine- prefix, we expect it to be stripped
    //
    string expected = ver_str;
    if (expected.compare (0, 5, "wine-") == 0)
      expected = expected.substr (5);
    assert (v->to_string() == expected);

    // Re-parse the string representation
    //
    auto v2 = wine_parse_version (v->to_string());
    assert (v2);
    assert (*v2 == *v);
  }
}

int main ()
{
  test_version ();
  test_version_comparison ();
  test_version_constructor ();
  test_version_roundtrip ();
}
