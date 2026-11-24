#include "ww_qol_config_3ds.h"
#include "ww_ui_shortcut_config_3ds.h"
#include "ww_ui_lifecycle_3ds.h"
#include "ww_disc_header_3ds.h"
#include "ww_ciso_3ds.h"
#include "ww_exit_order_3ds.h"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
static FILE * config(const char * text) 
{
    FILE * f = std::tmpfile();
    assert(f);
    std::fwrite(text,
1,
std::strlen(text),
f);
    std::rewind(f);
    return f;
    
}
int main(int argc,
char * * argv) 
{
    assert(argc == 2);
    using namespace n3ds_ui;
    unsigned checked = 0;
    for (unsigned alpha=0;alpha<256;alpha++)
    for (unsigned drawn=0;drawn<32;drawn++)
    for (unsigned usable=0;usable<32;usable++)
    for (unsigned block=0;block<16;block++) 
    {
        auto p = presentation(alpha,
drawn,
usable,
block);
        assert(p.alpha() == (block?0:alpha));
        assert(p.drawn() == (!block 
&& alpha?drawn:0));
        assert(p.usable() == (!block 
&& alpha == 255?(drawn&usable):0));
        assert(p.blocked() == block);
        ++checked;
        
    }
    ShortcutConfig profile;
    FILE * f = config("slot1.action = Y\nslot2.action = X\nslot3.action = NONE\n");
    assert(profile.read(f) == 0);
    std::fclose(f);
    assert(profile.bindings[0].action == N3DS_UI_ITEM_Y);
    assert(profile.bindings[1].action == N3DS_UI_ITEM_X);
    assert(profile.bindings[2].action == 0);
    for (const char* bad:{"slot4.action=X\n",
"slot1.x=0\n",
"slot1.action=MENU\n",
"slot9.action=X\n"
}
) 
{
    f = config(bad);
    assert(profile.read(f) > 0);
    std::fclose(f);
    assert(profile.bindings[0].action == N3DS_UI_ITEM_Y);
    
}
f = config("slot1.action=X\nslot2.action=X\n");
assert(profile.read(f) == 0);
std::fclose(f);
Touch touch;
assert(touch.update(true,
250,
30,
allActions,
false,
profile.bindings) == N3DS_UI_ITEM_X);
assert(touch.pressedSlot() == 0);
assert(touch.update(true,
250,
90,
allActions,
false,
profile.bindings) == 0);
assert(touch.update(true,
250,
30,
allActions,
false,
profile.bindings) == 0);
touch.update(false,
0,
0,
allActions,
false,
profile.bindings);
assert(touch.update(true,
250,
90,
allActions,
false,
profile.bindings) == N3DS_UI_ITEM_X);
assert(touch.pressedSlot() == 1);
touch.invalidate(N3DS_UI_ITEM_X);
assert(touch.update(true,
250,
90,
allActions,
false,
profile.bindings) == 0);
touch.update(false,
0,
0,
0);
assert(touch.update(true,
20,
210,
allActions,
true) == 0);
assert(hit(319,
239,
allActions) == 0);
assert(actionsForItems(allActions,
0xffffff) == N3DS_UI_MAP_SIZE);
assert(actionsForItems(allActions,
0xffff01)&N3DS_UI_ITEM_X);
n3ds_qol::Profile qos;
auto load = [&](const std::string& text) 
{
    FILE * file = std::fopen(argv[1],
"w");
    assert(file);
    std::fwrite(text.data(),
1,
text.size(),
file);
    std::fclose(file);
    return n3ds_qol::load(argv[1],
qos);
    
} ;
for (const char* sail:{"original",
"swift",
"wind"
}
)
for (unsigned mask=0;mask<16;mask++) 
{
    std::string text = std::string("sail=") + sail + "\n";
    const char * names[] = 
    {
        "fast_grapple", "no_song_replay", "instant_text", "gyro"
    } ;
    for (unsigned i=0;i<4;i++)text += std::string(names[i]) + "=" + char('0' + ((mask >> i)&1)) + "\n";
    assert(load(text) == 0);
    assert(qos.fastGrapple == bool(mask&1) 
&& qos.skipSongReplay == bool(mask&2) 
&& qos.instantLines == bool(mask&4) 
&& qos.motionAim == bool(mask&8));
    
}
assert(load("sail=swift\nfast_grapple=1\n") == 0);
for (const char* bad:{"unknown=1\n",
"sail=wind\nsail=original\n",
"gyro=2\n",
"gyro_sensitivity=24\n",
"gyro_sensitivity=301\n",
"gyro_sensitivity=nan\n"
}
) 
{
    assert(load(bad) > 0);
    assert(!std::strcmp(qos.sailMode,
"swift") 
&& qos.fastGrapple);
    
}
assert(load(std::string(300,
'x')) > 0);
for (int sensitivity:{25,
100,
300}) 
{
    assert(load("gyro=1\ngyro_sensitivity=" + std::to_string(sensitivity) + "\n") == 0);
    assert(qos.motionAim 
&& qos.motionScale == sensitivity);
    
}
assert(load("gyro_invert_x=1\ngyro_invert_y=1\n") == 0);
assert(qos.motionFlipX 
&& qos.motionFlipY);
std::remove(argv[1]);

{
    unsigned char gzle[0x20]
    {
        
    } ;
    std::memcpy(gzle,
"GZLE",
4);
    gzle[0x1C] = 0xC2;
    gzle[0x1D] = 0x33;
    gzle[0x1E] = 0x9F;
    gzle[0x1F] = 0x3D;
    assert(n3dsDiscHeaderKind(gzle,
sizeof gzle) == 1);
    unsigned char ciso[8]
    {
        'C',
'I',
'S',
'O',
0,
0,
0,
0
    } ;
    assert(n3dsDiscHeaderKind(ciso,
sizeof ciso) == 2);
    unsigned char junk[0x20]
    {
        'G',
'Z',
'L',
'P'
    } ;
    assert(n3dsDiscHeaderKind(junk,
sizeof junk) == 0);
    assert(n3dsDiscHeaderKind(nullptr,
0) == 0);
    assert(n3dsDiscHeaderKind(gzle,
4) == 0);
    
}
{
    unsigned char header[kN3dsCisoHeaderSize]
    {
        
    } ;
    std::memcpy(header,
"CISO",
4);
    const uint32_t block = 0x800;
    std::memcpy(header + 4,
&block,
4);
    header[8] = 1;
    header[9] = 0;
    header[10] = 1;
    std::vector < unsigned char > raw(block, 'A');
    std::vector < unsigned char > raw2(block, 'C');
    FILE * f = std::tmpfile();
    assert(f);
    std::fwrite(header,
1,
sizeof header,
f);
    std::fwrite(raw.data(),
1,
raw.size(),
f);
    std::fwrite(raw2.data(),
1,
raw2.size(),
f);
    std::rewind(f);
    N3dsCisoMap map;
    assert(map.load(header,
sizeof header));
    unsigned char buf[0x800];
    assert(n3dsCisoReadFile(f,
&map,
0,
buf,
sizeof buf));
    assert(buf[0] == 'A' 
&& buf[0x7FF] == 'A');
    assert(n3dsCisoReadFile(f,
&map,
block,
buf,
sizeof buf));
    assert(buf[0] == 0 
&& buf[0x7FF] == 0);
    assert(n3dsCisoReadFile(f,
&map,
block * 2,
buf,
4));
    assert(buf[0] == 'C' 
&& buf[3] == 'C');
    std::fclose(f);
    
}
{
    N3dsExitState exit;
    assert(!exit.readyForHome());
    assert(exit.mark(N3DS_EXIT_PREPARE));
    assert(!exit.mark(N3DS_EXIT_PREPARE));
    assert(exit.mark(N3DS_EXIT_GPU));
    assert(!exit.mark(N3DS_EXIT_GPU));
    assert(exit.mark(N3DS_EXIT_GFX));
    assert(!exit.readyForHome());
    assert(exit.mark(N3DS_EXIT_APT));
    assert(exit.readyForHome());
    assert(exit.has(N3DS_EXIT_GFX) 
&& exit.has(N3DS_EXIT_APT));
    
}
std::printf("PASS %u lifecycle states; touch cancellation and duplicate assignments; transactional shortcut/QoL parsing; disc header and exit order\n",
checked);

}
