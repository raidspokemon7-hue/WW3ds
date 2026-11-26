#pragma once
#include <stdlib.h>
#include <cstdio>
#include <cstring>
extern "C" void n3dsUiSetTopHudEnabled(int) __attribute__((weak));
struct Performance3ds 
{
    int spareCore = 1;
    bool fixedUniforms = true;
    bool reusePlans = true;
    bool cacheMaterials = true;
    bool cacheGeometry = true;
    bool prepareFog = true;
    bool coalesceVertices = true;
    bool planVertices = true;
    bool decodeVertices = true;
    bool cachePalettes = true;
    bool lowerScreen = true;
    bool widePicture = true, upperHud = true, showFrameRate = false;
    bool timeDraws = false;
    bool shareMaterials = false;
    bool deltaUniforms = false;
    bool programsForModels = false;
    bool readyMaterials = false;
    bool materialIdsOnModels = false;
    bool snapshotDirect = false;
    bool directModels = false;
    bool templateStates = false;
    bool incrementalCpState = false;
    bool pieceUniforms = false;
    int directSlotCount = 128;
    bool eventDirect = false;
    bool readyShapes = false;
    bool readyDraw = false;
    bool recordMaterials = false;
    bool recordModels = false;
    bool compactPoses = false;
    bool blockCommands = false;
    bool stageEight = true;
    bool slimState = true;
    int retainLimitKb = 4096;
    bool keepGeometry = true;
    bool inplaceState = true;
    bool cullWide = true;
    bool commandThread = true;
    bool systemCoreThreads = true;
    int commandCore = 3;
    bool helperDecode = true;
    int helperCore = 1;
    bool fenceHeap = true;
    bool workerPackets = false, verifyPackets = false;
    int allocSpanCount = 128;
    bool quickAlloc = true;
    bool reuseDraws = false;
    int reuseDrawKb = 2560;
    bool foldDepth = true;
    bool deltaPlans = true;
    bool soundOnCore1 = true;
    int backgroundCore = 0;
    bool reverseDepth = true;
    bool useDepthZ = true;
    int farFoliage = 2500;
    bool j3dMeshes = true;
    bool j3dShaders = false;
    bool splitTextures = true;
    bool packedTextures = true;
    bool stayGeometry = true;
    bool replayDraws = true;
    int replayLimitKb = 4096;
    bool flushWholeFrame = false;
    bool recordDraws = true;
    int residentLimitKb = 2048;
    int packetCore = 1;
    bool tightRecordGrowth = false;
    bool sectionUniforms = true;
    bool compactRecords = true;
    bool errorTable = false;
    bool overwriteDecode = false;
    bool templatePlans = false;
    bool batchMatrices = true;
    bool packDirect = false;
    bool packPlanar = false;
    int autoSplit = 0;
    bool vertexMajorPack = false;
    bool partialStaging = true;
    int kickLimit = 48;
    int splitLimit = 2;
    bool keepPlanNotes = false;
    bool fifoVertexPool = false;
    bool poolState = false;
    bool incrementalState = true;
    bool skipSnapshots = true;
    bool copySnapshots = true;
    bool releaseEarly = true;
    bool reuseCopies = false;
    bool shadowsOn = false;
    bool omitDraw = false;
    int skippedFrames = 0;
    bool clipRigid = false;
    bool fractionalPaint = true;
    int pacePaint = 1;
    int maxPaintGap = 2;
    bool pacePresent = false;
    int cadencePaint = 0;
    bool fxBatch = true;
    int floorPool = 512;
    bool textureDirect = true;
    bool typedCommands = false;
    bool mapOnLower = false;
    unsigned rejectedLines = 0;
    static char * trim(char * s)
    {
        while( * s == ' ' || * s == '\t')++s;
        size_t n = std::strlen(s);
        while(n 
&& (s[n - 1] == ' ' || s[n - 1] == '\t' || s[n - 1] == '\r' || s[n - 1] == '\n'))s[--n] = 0;
        return s;
        
    }
    void read(FILE * f)
    {
        char line[256];
        while(std::fgets(line,
sizeof line,
f))
        {
            if(!std::strchr(line,
'\n') 
&& !std::feof(f))
            {
                int c;
                while((c = std::fgetc(f)) != '\n' 
&& c != EOF)
                {
                    
                }
                ++rejectedLines;
                continue;
                
            }
            if(char * c = std::strpbrk(line,
"#;")) * c = 0;
            char * key = trim(line);
            if(! * key)continue;
            char * eq = std::strchr(key,
'=');
            if(!eq)
            {
                ++rejectedLines;
                continue;
                
            }
             * eq = 0;
            key = trim(key);
            char * val = trim(eq + 1);
            if(!std::strcmp(key,
"core2") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))spareCore = * val - '0';
            else if(!std::strcmp(key,
"material_cache") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))cacheMaterials = * val == '1';
            else if(!std::strcmp(key,
"packed_uniforms") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))fixedUniforms = * val == '1';
            else if(!std::strcmp(key,
"plan_reuse") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))reusePlans = * val == '1';
            else if(!std::strcmp(key,
"fog_prepare") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))prepareFog = * val == '1';
            else if(!std::strcmp(key,
"vertex_plan") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))planVertices = * val == '1';
            else if(!std::strcmp(key,
"bottom_ui") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))lowerScreen = * val == '1';
            else if(!std::strcmp(key,
"palette_cache") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))cachePalettes = * val == '1';
            else if(!std::strcmp(key,
"vertex_decode") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))decodeVertices = * val == '1';
            else if(!std::strcmp(key,
"draw_timing") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))timeDraws = * val == '1';
            else if(!std::strcmp(key,
"pipeline_stage8") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))stageEight = * val == '1';
            else if(!std::strcmp(key,
"geometry_cache") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))cacheGeometry = * val == '1';
            else if(!std::strcmp(key,
"lean_state") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))slimState = * val == '1';
            else if(!std::strcmp(key,
"state_inplace") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))inplaceState = * val == '1';
            else if(!std::strcmp(key,
"retain_kb") 
&& std::atoi(val) >= 256 
&& std::atoi(val) <= 16384)retainLimitKb = std::atoi(val);
            else if(!std::strcmp(key,
"retain_geometry") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))keepGeometry = * val == '1';
            else if(!std::strcmp(key,
"vertex_runs") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))coalesceVertices = * val == '1';
            else if(!std::strcmp(key,
"widescreen") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))widePicture = * val == '1';
            else if(!std::strcmp(key,
"frame_counter") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))showFrameRate = * val == '1';
            else if(!std::strcmp(key,
"wide_cull") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))cullWide = * val == '1';
            else if(!std::strcmp(key,
"heap_fence") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))fenceHeap = * val == '1';
            else if(!std::strcmp(key,
"fifo_thread") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))commandThread = * val == '1';
            else if(!std::strcmp(key,
"fifo_core") 
&& val[0] >= '0' 
&& val[0] <= '3' 
&& !val[1])commandCore = * val - '0';
            else if(!std::strcmp(key,
"decode_helper") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))helperDecode = * val == '1';
            else if(!std::strcmp(key,
"decode_core") 
&& val[0] >= '0' 
&& val[0] <= '3' 
&& !val[1])helperCore = * val - '0';
            else if(!std::strcmp(key,
"core1_threads") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))systemCoreThreads = * val == '1';
            else if(!std::strcmp(key,
"packet_worker") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))workerPackets = * val == '1';
            else if(!std::strcmp(key,
"fast_alloc_spans") 
&& atoi(val) >= 32 
&& atoi(val) <= 160)allocSpanCount = atoi(val);
            else if(!std::strcmp(key,
"fast_alloc") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))quickAlloc = * val == '1';
            else if(!std::strcmp(key,
"draw_reuse") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))reuseDraws = * val == '1';
            else if(!std::strcmp(key,
"draw_reuse_kb") 
&& atoi(val) >= 256 
&& atoi(val) <= 8192)reuseDrawKb = atoi(val);
            else if(!std::strcmp(key,
"depth_fold") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))foldDepth = * val == '1';
            else if(!std::strcmp(key,
"plan_delta") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))deltaPlans = * val == '1';
            else if(!std::strcmp(key,
"bg_core") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1") || !std::strcmp(val,
"2") || !std::strcmp(val,
"3")))backgroundCore = * val - '0';
            else if(!std::strcmp(key,
"depth_reverse") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))reverseDepth = * val == '1';
            else if(!std::strcmp(key,
"depth_z") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))useDepthZ = * val == '1';
            else if(!std::strcmp(key,
"snapshot_skip") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))skipSnapshots = * val == '1';
            else if(!std::strcmp(key,
"snapshot_copies") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))copySnapshots = * val == '1';
            else if(!std::strcmp(key,
"audio_core1") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))soundOnCore1 = * val == '1';
            else if(!std::strcmp(key,
"early_release") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))releaseEarly = * val == '1';
            else if(!std::strcmp(key,
"copy_reuse") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))reuseCopies = * val == '1';
            else if(!std::strcmp(key,
"real_shadows") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))shadowsOn = * val == '1';
            else if(!std::strcmp(key,
"skip_draw") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))omitDraw = * val == '1';
            else if(!std::strcmp(key,
"frame_skip"))
            {
                const int v = atoi(val);
                if(v >= 0 
&& v <= 3)skippedFrames = v;
                else ++rejectedLines;
                
            }
            else if(!std::strcmp(key,
"rigid_clip") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))clipRigid = * val == '1';
            else if(!std::strcmp(key,
"paint_fractional") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))fractionalPaint = * val == '1';
            else if(!std::strcmp(key,
"paint_pacing"))
            {
                const int v = atoi(val);
                if(v >= 0 
&& v <= 1)pacePaint = v;
                else ++rejectedLines;
                
            }
            else if(!std::strcmp(key,
"paint_max_gap"))
            {
                const int v = atoi(val);
                if(v >= 2 
&& v <= 8)maxPaintGap = v;
                else ++rejectedLines;
                
            }
            else if(!std::strcmp(key,
"present_pace") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))pacePresent = * val == '1';
            else if(!std::strcmp(key,
"paint_cadence"))
            {
                const int v = atoi(val);
                if(v >= 0 
&& v <= 4)cadencePaint = v;
                else ++rejectedLines;
                
            }
            else if(!std::strcmp(key,
"batch_fx") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))fxBatch = * val == '1';
            else if(!std::strcmp(key,
"pool_floor"))
            {
                const int v = atoi(val);
                if(v >= 0 
&& v <= 768)floorPool = v;
                else ++rejectedLines;
                
            }
            else if(!std::strcmp(key,
"foliage_far"))
            {
                const int v = atoi(val);
                if(v >= 0 
&& v <= 100000)farFoliage = v;
                else ++rejectedLines;
                
            }
            else if(!std::strcmp(key,
"j3d_geometry") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))j3dMeshes = * val == '1';
            else if(!std::strcmp(key,
"j3d_programs") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))j3dShaders = * val == '1';
            else if(!std::strcmp(key,
"texture_shards") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))splitTextures = * val == '1';
            else if(!std::strcmp(key,
"texture_pack") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))packedTextures = * val == '1';
            else if(!std::strcmp(key,
"resident_geometry") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))stayGeometry = * val == '1';
            else if(!std::strcmp(key,
"shared_materials") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))shareMaterials = * val == '1';
            else if(!std::strcmp(key,
"uniform_delta") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))deltaUniforms = * val == '1';
            else if(!std::strcmp(key,
"model_programs") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))programsForModels = * val == '1';
            else if(!std::strcmp(key,
"prepared_materials") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))readyMaterials = * val == '1';
            else if(!std::strcmp(key,
"model_material_ids") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))materialIdsOnModels = * val == '1';
            else if(!std::strcmp(key,
"direct_snapshot") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))snapshotDirect = * val == '1';
            else if(!std::strcmp(key,
"model_direct") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))directModels = * val == '1';
            else if(!std::strcmp(key,
"state_templates") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))templateStates = * val == '1';
            else if(!std::strcmp(key,
"state_cp_incremental") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))incrementalCpState = * val == '1';
            else if(!std::strcmp(key,
"uniform_pieces") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))pieceUniforms = * val == '1';
            else if(!std::strcmp(key,
"direct_slots") 
&& (!std::strcmp(val,
"128") || !std::strcmp(val,
"256") || !std::strcmp(val,
"512")))directSlotCount = std::atoi(val);
            else if(!std::strcmp(key,
"direct_events") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))eventDirect = * val == '1';
            else if(!std::strcmp(key,
"prepared_shapes") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))readyShapes = * val == '1';
            else if(!std::strcmp(key,
"prepared_draw") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))readyDraw = * val == '1';
            else if(!std::strcmp(key,
"material_records") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))recordMaterials = * val == '1';
            else if(!std::strcmp(key,
"model_records") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))recordModels = * val == '1';
            else if(!std::strcmp(key,
"compact_pose") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))compactPoses = * val == '1';
            else if(!std::strcmp(key,
"command_blocks") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))blockCommands = * val == '1';
            else if(!std::strcmp(key,
"draw_record") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))recordDraws = * val == '1';
            else if(!std::strcmp(key,
"frame_flush_all") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))flushWholeFrame = * val == '1';
            else if(!std::strcmp(key,
"replay_kb") 
&& atoi(val) >= 256 
&& atoi(val) <= 8192)replayLimitKb = atoi(val);
            else if(!std::strcmp(key,
"draw_replay") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))replayDraws = * val == '1';
            else if(!std::strcmp(key,
"resident_kb") 
&& atoi(val) >= 256 
&& atoi(val) <= 8192)residentLimitKb = atoi(val);
            else if(!std::strcmp(key,
"packet_worker_core") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))packetCore = * val - '0';
            else if(!std::strcmp(key,
"packet_worker_verify") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))verifyPackets = * val == '1';
            else if(!std::strcmp(key,
"record_tight_growth") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))tightRecordGrowth = * val == '1';
            else if(!std::strcmp(key,
"uniform_sections") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))sectionUniforms = * val == '1';
            else if(!std::strcmp(key,
"record_compact") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))compactRecords = * val == '1';
            else if(!std::strcmp(key,
"tex_error_table") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))errorTable = * val == '1';
            else if(!std::strcmp(key,
"decode_overwrite") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))overwriteDecode = * val == '1';
            else if(!std::strcmp(key,
"plan_templates") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))templatePlans = * val == '1';
            else if(!std::strcmp(key,
"matrix_batch") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))batchMatrices = * val == '1';
            else if(!std::strcmp(key,
"planar_pack") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))packPlanar = * val == '1';
            else if(!std::strcmp(key,
"auto_ab") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))autoSplit = * val == '1';
            else if(!std::strcmp(key,
"pack_vertex_major") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))vertexMajorPack = * val == '1';
            else if(!std::strcmp(key,
"staging_partial") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))partialStaging = * val == '1';
            else if(!std::strcmp(key,
"gpu_kick"))
            {
                const int v = atoi(val);
                if(v >= 0 
&& v <= 4000)kickLimit = v;
                else ++rejectedLines;
                
            }
            else if(!std::strcmp(key,
"gpu_split") 
&& atoi(val) >= 1 
&& atoi(val) <= 50)splitLimit = atoi(val);
            else if(!std::strcmp(key,
"direct_pack") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))packDirect = * val == '1';
            else if(!std::strcmp(key,
"plan_notes") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))keepPlanNotes = * val == '1';
            else if(!std::strcmp(key,
"vertex_pool_fifo") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))fifoVertexPool = * val == '1';
            else if(!std::strcmp(key,
"state_incremental") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))incrementalState = * val == '1';
            else if(!std::strcmp(key,
"state_pool") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))poolState = * val == '1';
            else if(!std::strcmp(key,
"direct_texture") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))textureDirect = * val == '1';
            else if(!std::strcmp(key,
"typed_gx") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))typedCommands = * val == '1';
            else if(!std::strcmp(key,
"top_hud") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))
            {
                upperHud = * val == '1';
                if(n3dsUiSetTopHudEnabled)n3dsUiSetTopHudEnabled(upperHud);
                
            }
            else if(!std::strcmp(key,
"ui_map_bottom") 
&& (!std::strcmp(val,
"0") || !std::strcmp(val,
"1")))mapOnLower = * val == '1';
            else ++rejectedLines;
            
        }
        applyPreset();
        
    }
    void applyPreset()
    {
        spareCore = 1;
        commandThread = true;
        commandCore = 3;
        systemCoreThreads = true;
        soundOnCore1 = true;
        stageEight = true;
        fenceHeap = true;
        keepGeometry = true;
        replayDraws = true;
        deltaPlans = true;
        reusePlans = true;
        fractionalPaint = true;
        coalesceVertices = true;
        cacheMaterials = true;
        fixedUniforms = true;
        prepareFog = true;
        planVertices = true;
        decodeVertices = true;
        cachePalettes = true;
        textureDirect = true;
        cullWide = true;
        slimState = true;
        inplaceState = true;
        batchMatrices = true;
        cacheGeometry = true;
        clipRigid = false;
        typedCommands = false;
        timeDraws = false;
        widePicture = true;
        upperHud = true;
        lowerScreen = true;
        mapOnLower = false;
        showFrameRate = true;
        packedTextures = true;
        splitTextures = true;
        
    }
    static const char * example()
    {
        return "# Fidelity-preserving optimizations only. Restart after editing.\n# Tree/grass draw distance in game units (default 2500). Islands/horizon unchanged. 3000=farther, 1200=more pop-in.\nfoliage_far=2500\n# Real shadows: 0=off (faster, default), 1=on\nreal_shadows=0\n# Core-2 rendering: 0=off, 1=on (default)\ncore2=1\n# Exact decoded-geometry reuse, bounded 3MiB: 0=off, 1=on (default)\ngeometry_cache=1\n# Exact pre-decode primitive coalescing: 0=off, 1=on (default)\nvertex_runs=1\n# Exact consecutive-material cache shortcut: 0=off, 1=on\nmaterial_cache=1\n# Reusable draw plans: 0=off, 1=on (default)\nplan_reuse=1\n# Fixed uniform storage: 0=off, 1=on (default)\npacked_uniforms=1\n# Identical fog preparation: 0=off, 1=on (default)\nfog_prepare=1\n# Cached vertex-decode recipes: 0=off, 1=on\nvertex_plan=1\n# Specialized exact attribute loops: 0=off, 1=on\nvertex_decode=1\n# Exact palette hash memo: 0=off, 1=on\npalette_cache=1\n# Bottom status panel: 0=blank, 1=panel\nbottom_ui=1\n# Detailed GPU timings: 0=logged frames only, 1=every frame\ndraw_timing=0\n" "# Bounded core2 staging: 0=2MiB, 1=8MiB (default1)\npipeline_stage8=1\n" "# Full-width widescreen culling correction: 0=reference, 1=on\nwide_cull=1\n" "# Secondary threads (audio, loading) on core 1: 0=off, 1=on (default1)\ncore1_threads=1\n" "# GX command translation on its own thread: 0=off (on the game thread), 1=on (default1)\nfifo_thread=1\n# Core of that thread: 3=spare New 3DS core (default3), 1=shared system core\nfifo_core=3\n" "# Parallel vertex decode on another core (same pixels): 0=off, 1=on (default1). Needs fifo_thread=1. Falls back off if the thread will not start.\ndecode_helper=1\n# Core for that helper: 1=system core (default), 0=main. Never use the fifo or render core.\ndecode_core=1\n" "# Core-2 staging waits only when the real heap is short: 0=estimate, 1=real heap (default1)\nheap_fence=1\n" "# Direct SDK register delivery, inline FIFO and CPU recorder only (experimental)\ntyped_gx=0\n# Exact direct texture encoding, 0=reference host upload, 1=default\ndirect_texture=1\n# Experimental bounded TEV/texgen vector-capacity reuse\nstate_pool=0\n# Experimental FIFO geometry-capacity reuse, same memory budget\nvertex_pool_fifo=0\n# GPU plan diagnostic text: 1=reference, 0=omit unread notes (effects unchanged)\nplan_notes=0\n# Experimental direct packing into the existing GPU arena\ndirect_pack=0\n# Adjacent matrix-palette grouping: 0=off, 1=on (default)\nmatrix_batch=1\n# Reuse unchanged geometry: 0=off, 1=on (default)\nretain_geometry=1\n# Reuse validated GPU commands: 0=off, 1=on (default)\ndraw_replay=1\n# Reuse matching draw plans: 0=off, 1=on (default)\nplan_delta=1\n# Pace from measured frame cost: 0=off, 1=on (default)\npaint_fractional=1\n# Skip actor draws when behind: 0=off (default, avoids blink), 1=on\nskip_draw=0\n# Extra non-painted logic frames when behind: 0=off (default), 1-3=on\nframe_skip=0\n# Audio thread on core 1: 0=off, 1=on (default)\naudio_core1=1\n# Top screen: 0=original 4:3, 1=full width (default)\nwidescreen=1\n# Bottom screen: 0=blank, 1=chart. Live map stays off.\nbottom_ui=1\nui_map_bottom=0\n# Skip fully offscreen meshes: 0=off (default; 1 can false-cull under moving cameras)\nrigid_clip=0\n# Use a prepared texture pack when one is on the card: 0=always convert while playing, 1=default\ntexture_pack=1\ntexture_shards=1\n# Frame counter: 0=off, 1=on (default)\nframe_counter=1\n# Experimental bounded immutable GPU plan templates\nplan_templates=0\n# Experimental fully-overwritten decoded scalar buffers\ndecode_overwrite=0\n# Top HUD uses the full 400px width (hearts/buttons/rupees larger, in the corners): 0=original, 1=on (default1)\ntop_hud=1\n";
        
    }
    
} ;
